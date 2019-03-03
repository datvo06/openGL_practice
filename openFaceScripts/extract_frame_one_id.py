from __future__ import print_function, unicode_literals
import sys
import os
import csv
import cv2
import numpy as np

from auto_script import get_face_bbox


def get_intersection(x_start1, x_start2, x_end1, x_end2):
    if x_start1 > x_start2:
        x_start1, x_end1, x_start2, x_end2 = x_start2, x_end2, x_start1, x_end1
    return min(max(0, x_end1 - x_start2), x_end2 - x_start2)


def IOU(bbox1, bbox2):
    area_intersect = get_intersection(bbox1[0], bbox2[0], bbox1[2], bbox2[2])*\
        get_intersection(bbox1[1], bbox2[1], bbox1[3], bbox2[3])
    area_union = (bbox1[2] - bbox1[0]) * (bbox1[3] - bbox1[1]) +\
        (bbox2[2] - bbox2[0]) * (bbox2[3] - bbox2[1])
    return area_intersect/(area_union - area_intersect)


def get_max_bbox_contains_all(bbox_list):
    x_min = max(min([bbox[0] for bbox in bbox_list]), 0)
    x_max = max([bbox[2] for bbox in bbox_list])
    y_min = max(min([bbox[1] for bbox in bbox_list]), 0)
    y_max = min([bbox[2] for bbox in bbox_list])
    return (x_min, y_min, x_max, y_max)


def get_max_area(bbox_list):
    return max([
        (bbox[2] - bbox[0]) * (bbox[3] - bbox[1])
        for bbox in bbox_list])


def scale_bbox(bbox, ratio):
    width = bbox[2] - bbox[0]
    height = bbox[3] - bbox[1]
    middle_point = (int((bbox[0]+bbox[2])/2), int((bbox[1]+bbox[3])/2))
    new_width = int(width * ratio)
    new_height = int(height * ratio)
    return (max(int(middle_point[0] - int(new_width/2)), 0),
            max(int(middle_point[1] - int(new_height/2)), 0),
            int(middle_point[0] + int(new_width/2)),
            int(middle_point[1] + int(new_height/2)))


def get_all_id_frame(in_video_path, person_id, out_folder):
    if not os.path.exists(out_folder):
        os.makedirs(out_folder)
    video_filename_full = os.path.split(in_video_path)[1]
    video_filename, video_ext = os.path.splitext(video_filename_full)
    csv_file_path = os.path.join('./processed', video_filename + '.csv')
    reader = csv.reader(open(csv_file_path, 'r'))
    x_index_start = 0
    y_index_start = 0

    last_bbox = [0, 0, 1, 1]
    face_bboxs_list = []
    for row_index, row in enumerate(reader):
        if row_index == 0:
            # Check the position of the bbox and the id
            for elem_index, elem in enumerate(row):
                if (''.join(elem.split())).startswith("x_"):
                    x_index_start = elem_index
                    y_index_start = elem_index + 68
                    break
        else:
            if int(row[1]) != person_id:
                continue
            # Check the iou on the last bbox, if IOU < 70, creates new frame
            # and compile all the previous to a list
            new_face = []
            for point_index in range(68):
                new_face.append(
                    (float(row[x_index_start + point_index]),
                     float(row[y_index_start + point_index]))
                )
            bbox = get_face_bbox(new_face)
            bbox = (int(bbox[0]), int(bbox[1]),
                    int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
            if IOU(bbox, last_bbox) < 0.7:
                if len(face_bboxs_list) > 0:
                    if len(face_bboxs_list[-1]) < 100:
                        face_bboxs_list.pop()
                face_bboxs_list.append([(int(row[0]), bbox)])
            else:
                face_bboxs_list[-1].append((int(row[0]), bbox))
            last_bbox = bbox
    areas = [get_max_area([elem[1] for elem in elem_list])
             for elem_list in face_bboxs_list]
    max_bboxs = [get_max_bbox_contains_all([elem[1] for elem in elem_list])
                 for elem_list in face_bboxs_list]
    for i, bbox in enumerate(max_bboxs):
        height = bbox[2] - bbox[0]
        width = bbox[3] - bbox[1]
        max_dim = max(height, width)
        bbox = (bbox[0], bbox[1], bbox[0] + max_dim, bbox[1]+max_dim)
        bbox_area = max_dim**2
        ratio = bbox_area / areas[i]
        if ratio < 9:
            bbox = scale_bbox(bbox, np.sqrt(9/ratio))
        max_bboxs[i] = bbox

    # Let's open the cap
    cap = cv2.VideoCapture(in_video_path)
    i = 0
    current_list_index = 0
    current_bbox_index = 0
    # Let's calculate bbox for each list
    while(cap.isOpened()):
        ret, frame = cap.read()
        i += 1
        if (current_list_index >= len(face_bboxs_list)):
            break
        if (current_bbox_index >= len(face_bboxs_list[current_list_index])):
            current_list_index += 1
            current_bbox_index = 0
        if (current_list_index >= len(face_bboxs_list)):
            break
        if i < face_bboxs_list[current_list_index][current_bbox_index][0]:
            continue

        # extract
        print("max bbox: ", max_bboxs[current_list_index])
        subimage = frame[max_bboxs[current_list_index][1]:max_bboxs[current_list_index][3],
                         max_bboxs[current_list_index][0]:max_bboxs[current_list_index][2]]
        out_dir_for_list = os.path.join(out_folder, str(current_list_index))
        if not (os.path.exists(out_dir_for_list)):
            os.makedirs(out_dir_for_list)
        cv2.imwrite(os.path.join(out_dir_for_list,
                                 str(face_bboxs_list[current_list_index][current_bbox_index][0]) + '.jpg'),
                    cv2.resize(subimage, (256, 256), interpolation=cv2.INTER_LINEAR))
        current_bbox_index += 1

if __name__ == '__main__':
    get_all_id_frame(sys.argv[1], int(sys.argv[2]), 'out_frames')
