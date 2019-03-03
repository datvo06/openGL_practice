from __future__ import print_function, division
import shutil
import os
import sys
import csv
import numpy as np
import cv2
import csv


def get_face_bbox(face):
    min_x = min([point[0] for point in face])
    max_x = max([point[0] for point in face])
    min_y = min([point[1] for point in face])
    max_y = max([point[1] for point in face])
    face_width = max_x - min_x + 1
    face_height = max_y - min_y + 1
    return (min_x, min_y, face_width, face_height)


def add_gaussian_deviation_to_face(face):
    new_face = []
    _, _, face_width, face_height = get_face_bbox(face)
    std_x = max(int(face_width/20), 1)
    std_y = max(int(face_height/20), 1)
    mean = np.array([0, 0])
    cov = np.array([[std_x, 0], [0, std_y]])
    x, y = np.random.multivariate_normal(mean, cov, 68).T
    for point_index, point in enumerate(face):
        new_face.append(
            (int(point[0]+x[point_index]), int(point[1]+x[point_index])))
    return new_face


def write_faces_to_file(faces, filepath):
    with open(filepath, "w") as out_file:
        for i, face in enumerate(faces):
            face = add_gaussian_deviation_to_face(face)
            print("Current face: ", i)
            out_file.write("face" + str(i+1) + '\n')
            out_file.write("{"+ '\n')
            for point_index, point in enumerate(face):
                out_file.write("{} {}\n".format(point[0], point[1]))
            out_file.write("}"+ '\n')


def read_whole_csv(reader, take_z=False, num_face=None):
    x_index_start = 0
    y_index_start = 0
    z_index_start = 0
    faces = []
    for row_index, row in enumerate(reader):
        if num_face is not None:
            if row_index > num_face:
                break
        if row_index == 0:
            for elem_index, elem in enumerate(row):
                prefix = "x_"
                if take_z:
                    prefix = "X_"
                if (''.join(elem.split())).startswith(prefix):
                    x_index_start = elem_index
                    y_index_start = elem_index + 68
                    z_index_start = y_index_start + 68
                    break
        else:
            # 3. Get all landmarks
            new_face = []
            for point_index in range(68):
                if not take_z:
                    new_face.append(
                        (
                            float(row[x_index_start + point_index]),
                            float(row[y_index_start + point_index])))
                else:
                    new_face.append(
                        (
                            float(row[x_index_start + point_index]),
                            float(row[y_index_start + point_index]),
                            float(row[z_index_start + point_index])
                        ))

            faces.append(new_face)
    return faces


def subprocessfile(filepath, dict_number_file):
    parent_dir, filename = os.path.split(filepath)
    filename_no_ext, filename_ext = os.path.splitext(filename)
    # 1. Copy file to the current directory
    shutil.copy(filepath, 'temp' + filename_ext)
    csv_file_path = os.path.join('.', 'processed',  'temp.csv')
    print("CSV file path: ", csv_file_path)

    # 2. Run facial landmark and get csv file
    try:
        os.system('./FaceLandmarkImg -f temp' + filename_ext)
        reader = csv.reader(open(csv_file_path, 'r'))
    except:
        return
    faces = read_whole_csv(reader)
    if len(faces) > 6:
        os.remove(os.path.join('temp' + filename_ext))
        shutil.rmtree('processed')
        return
    if len(faces) not in dict_number_file:
        dict_number_file[len(faces)] = 0
    else:
        dict_number_file[len(faces)] += 1
    if not os.path.exists(str(len(faces))):
        os.makedirs(str(len(faces)))

    if filename.endswith('jpg'):
        shutil.move('temp' + filename_ext, os.path.join(str(len(faces)), str(dict_number_file[len(faces)]) + '.jpg'))
    else:
        os.remove('temp' + filename_ext)
        img = cv2.imread(filepath)
        cv2.imwrite(os.path.join(str(len(faces)), str(dict_number_file[len(faces)]) + '.jpg'), img)

    write_faces_to_file(faces, os.path.join(str(len(faces)), str(dict_number_file[len(faces)]) + '.pts'))
    shutil.rmtree('processed')


def main_3(root_path):
    i = 0
    dict_number_file = {}
    list_files = []
    for (dirpath, dirnames, filenames) in os.walk(root_path):
        for filepath in filenames:
            filepath = os.path.join(dirpath, filepath)
            if filepath.endswith('jpg') or filepath.endswith('png'):
                list_files.append(filepath)
    filenames = [os.path.split(filepath)[1] for filepath in list_files]
    if not os.path.exists('final_dir'):
        os.makedirs('final_dir')
    import shutil
    for i, filename in enumerate(filenames):
        shutil.move(os.path.join('./replaced_images', str(i) + '.jpg'),
                    os.path.join('final_dir', filename))



def main(root_path):
    i = 0
    dict_number_file = {}
    for (dirpath, dirnames, filenames) in os.walk(root_path):
        for filepath in filenames:
            filepath = os.path.join(dirpath, filepath)
            if filepath.endswith('jpg') or filepath.endswith('png'):
                subprocessfile(filepath, dict_number_file)


def main_2(filename):
    # Let's extract every frame after having ran the openface
    cap = cv2.VideoCapture(filename)
    i = 0
    filename_no_ext, filename_ext = os.path.splitext(filename)
    os.system('./FaceLandmarkVidMulti -f ' + filename)
    if not os.path.exists('out_frames'):
        os.makedirs('out_frames')
    writer = csv.writer(open('filelist.csv', 'w'))
    writer.writerow(['ID', 'FILE', 'FACE_X', 'FACE_Y',
                  'FACE_WIDTH', 'FACE_HEIGHT'])
    reader = csv.reader(
        open(os.path.join('processed', filename_no_ext + '.csv')))
    faces = read_whole_csv(reader)
    print(len(faces))
    while(cap.isOpened()):
        try:
            ret, frame = cap.read()
            img_path = os.path.join('out_frames', str(i) + '.jpg')
            cv2.imwrite(img_path, frame)
            face_bboxs = get_face_bbox(faces[i])
            writer.writerow([str(i), img_path, str(face_bboxs[0]),
                        str(face_bboxs[1]),
                        str(face_bboxs[2]),
                        str(face_bboxs[3])])
            i += 1
        except:
            break

    cap.release()
    cv2.destroyAllWindows()


def main_4(filename):
    # Let's extract every frame after having ran the openface
    frame = cv2.imread(filename)
    i = 0
    filename_no_ext, filename_ext = os.path.splitext(filename)
    os.system('./FaceLandmarkImg -f ' + filename)
    if not os.path.exists('out_frames'):
        os.makedirs('out_frames')
    writer = csv.writer(open('filelist.csv', 'w'))
    writer.writerow(['ID', 'FILE', 'FACE_X', 'FACE_Y',
                  'FACE_WIDTH', 'FACE_HEIGHT'])
    reader = csv.reader(
        open(os.path.join('processed', filename_no_ext + '.csv')))
    faces = read_whole_csv(reader)
    print(len(faces))
    img_path = os.path.join('out_frames', str(i) + '.jpg')
    cv2.imwrite(img_path, frame)
    face_bboxs = get_face_bbox(faces[i])
    writer.writerow([str(i), img_path, str(face_bboxs[0]),
                str(face_bboxs[1]),
                str(face_bboxs[2]),
                str(face_bboxs[3])])
    i += 1


if __name__ == '__main__':
    main_3(sys.argv[1])
