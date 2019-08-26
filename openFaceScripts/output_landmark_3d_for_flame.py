from __future__ import print_function, unicode_literals
from auto_script import read_whole_csv
import sys
import os
import numpy as np
import csv
import pickle


def get_3d_landmarks(in_video_path, num_frame_max=1000):
    video_filename_full = os.path.split(in_video_path)[1]
    video_filename, video_ext = os.path.splitext(video_filename_full)
    csv_file_path = os.path.join('./processed', video_filename + '.csv')
    reader = csv.reader(open(csv_file_path, 'r'))
    faces = read_whole_csv(reader, take_z=True, num_face=num_frame_max)
    faces = np.array(faces)
    faces = faces[:, list(range(60))+list(range(61, 65)) + list(range(66,68)), :]/1000
    print(faces.shape)
    faces = faces[:, 17:, :]
    # faces[:, :, 2] /= 10.0
    '''
    faces[:, :, 1] /= 10.0
    faces[:, :, 2] /= 10.0
    faces[:, :, 1] -= np.mean(faces[:, :, 1])
    faces[:, :, 1] -= np.mean(faces[:, :, 1])
    '''
    pickle.dump(faces, open('output_landmark3d.pkl', 'wb'), protocol=2)


if __name__ == '__main__':
    get_3d_landmarks(sys.argv[1], 1000)
