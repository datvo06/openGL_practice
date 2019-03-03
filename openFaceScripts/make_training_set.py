from __future__ import print_function, unicode_literals
import sys
import os
import cv2
import numpy as np
import glob


def make_data_set(in_dir, out_dir):
    file_paths = glob.glob(in_dir + '/*.jpg')
    filenames_full = list(
        [os.path.split(file_path)[1]
         for file_path in file_paths])
    filenames = list(
        [os.path.splitext(filename_full)[0]
         for filename_full in filenames_full])
    framelists = list(
        sorted(int(filename) for filename in filenames))
    index_in_out_dir = 0
    i = 1
    while (i < len(framelists) - 1):
        if (framelists[i - 1] == framelists[i] - 1) and\
                (framelists[i + 1] == framelists[i] + 1):
            out_merged_frame = np.zeros((256, 256*3, 3))
            out_merged_frame[:, 0:256, :] = cv2.imread(os.path.join(
                in_dir, str(framelists[i - 1]) + '.jpg'))[:, :, :3]
            out_merged_frame[:, 256:512, :] = cv2.imread(os.path.join(
                in_dir, str(framelists[i]) + '.jpg'))[:, :, :3]
            out_merged_frame[:, 512:, :] = cv2.imread(os.path.join(
                in_dir, str(framelists[i + 1]) + '.jpg'))[:, :, :3]
            i += 2
            cv2.imwrite(os.path.join(out_dir, str(index_in_out_dir)) + '.jpg', out_merged_frame)
            index_in_out_dir += 1
        else:
            i += 1


if __name__ == '__main__':
    if not os.path.exists('training'):
        os.makedirs('training')
    for path in sys.argv[1:]:
        out_path = os.path.join('training', path)
        if not (os.path.exists(out_path)):
            os.makedirs(out_path)
        make_data_set(path, out_path)
