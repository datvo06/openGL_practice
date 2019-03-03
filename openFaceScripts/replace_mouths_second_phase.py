from __future__ import print_function, unicode_literals, division
import cv2
import numpy as np
import pickle
import os
from normalize_face_landmarks import get_top_left, get_bottom_right


def inside_contour_mask(img, contour):
    mask = np.zeros((img.shape[0], img.shape[1]))
    cv2.drawContours(mask, [contour], 0, 255, -1)
    return mask > 0


def contour_mask(img, contour):
    mask = np.zeros((img.shape[0], img.shape[1]))
    cv2.drawContours(mask, [contour], 0, 255, 3)
    return mask > 0



if __name__ == '__main__':
    # Load the map
    mapping = pickle.load(open('./map.pkl', 'rb'))
    # Load the train landmark
    landmarks_train = pickle.load(open('./land_marks_train.pkl', 'rb'))
    # Load the target landmark
    landmarks_target = pickle.load(open('./land_marks.pkl', 'rb'))
    # Load the train filepath
    train_file_list = pickle.load(open('./list_file_train.pkl', 'rb'))
    # Load the target filepath
    target_file_list = pickle.load(open('./list_file.pkl', 'rb'))
    if not os.path.exists("replaced_images"):
        os.makedirs("replaced_images")
    for i in range(len(target_file_list)):
        target_file_name = os.path.split(target_file_list[i])[1]
        target_img = cv2.imread(target_file_list[i][:-3] + "jpg")
        new_train_file_name = os.path.splitext(os.path.split(train_file_list[mapping[i]])[1])[0]
        new_train_file_name = os.path.join("1_train", new_train_file_name + ".jpg")
        print(new_train_file_name)
        train_img = cv2.imread(new_train_file_name)
        # Get the target bbox
        min_x, min_y = get_top_left(landmarks_target[i].reshape(-1, 2)[12:])
        max_x, max_y = get_bottom_right(landmarks_target[i].reshape(-1, 2)[12:])

        # Get the train bbox
        min_x_train, min_y_train = get_top_left(landmarks_train[mapping[i]].reshape(-1, 2)[12:])
        max_x_train, max_y_train = get_bottom_right(landmarks_train[mapping[i]].reshape(-1, 2)[12:])
        train_contour_mask = inside_contour_mask(train_img,
                                                 landmarks_train[mapping[i]].reshape(-1, 2)[12:])
        area = np.sum(train_contour_mask)
        print(area)
        if area < 200:
            cv2.imwrite(os.path.join("replaced_images",
                                     target_file_name[:-3]+"jpg"
                                 ), target_img)
            continue
        roi_mask = train_contour_mask[min_y_train:max_y_train,
                                      min_x_train:max_x_train]
        # Copy paste
        target_img[min_y:(min_y+max_y_train-min_y_train),
                   min_x:(min_x+max_x_train-min_x_train)][roi_mask] = train_img[min_y_train:max_y_train,
                                                                      min_x_train:max_x_train][roi_mask]
        inpaint_mask = contour_mask(train_img, landmarks_train[mapping[i]].reshape(-1, 2)[12:])
        inpaint_mask = inpaint_mask[min_y:(min_y+max_y_train-min_y_train),
                                    min_x:(min_x+max_x_train-min_x_train)]
        new_inpaint_mask = np.zeros((target_img.shape[0], target_img.shape[1])).astype(np.uint8)
        new_inpaint_mask[min_y:(min_y+max_y_train-min_y_train),
                   min_x:(min_x+max_x_train-min_x_train)][inpaint_mask] = 255
        target_img = cv2.inpaint(target_img, new_inpaint_mask, 3, cv2.INPAINT_NS)
        cv2.imwrite(os.path.join("replaced_images", target_file_name[:-3]+"jpg"
                                 ), target_img)
    pass
