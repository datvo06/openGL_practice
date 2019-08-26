from __future__ import print_function, unicode_literals
import sys
import os
import cv2
import numpy as np
import glob
import shutil

def merge_training_set(in_dir, out_dir, index):
    file_paths = glob.glob(in_dir + '/*.jpg')
    filenames_full = list(
        [os.path.split(file_path)[1]
         for file_path in file_paths])
    filenames = list(
        [os.path.splitext(filename_full)[0]
         for filename_full in filenames_full])
    framelists = list(
        sorted(int(filename) for filename in filenames))
    for count, i in enumerate(framelists):
        shutil.move(os.path.join(in_dir, str(i) + ".jpg"),
                    os.path.join(out_dir, str(index+count) + ".jpg"))
    index += len(framelists)
    return index


if __name__ == '__main__':
    if not os.path.exists('training'):
        os.makedirs('training')
    index = 0
    for path in sys.argv[1:]:
        out_path = 'training'
        if not (os.path.exists(out_path)):
            os.makedirs(out_path)
        index = merge_training_set(path, out_path, index)
