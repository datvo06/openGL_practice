from __future__ import print_function, division, unicode_literals
import sys
import pickle
import numpy as np


def get_top_left(mouth):
    min_x = np.amin(mouth[:, 0])
    min_y = np.amin(mouth[:, 1])
    return min_x, min_y


def get_bottom_right(mouth):
    min_x = np.amax(mouth[:, 0])
    min_y = np.amax(mouth[:, 1])
    return min_x, min_y


def get_mouth_normalized(mouth, starting_index = 0):
    mouth = mouth.reshape((20, 2))
    min_x, min_y = get_top_left(mouth[starting_index:, :])
    mouth[starting_index:, 0] -= min_x
    mouth[starting_index:, 1] -= min_y
    return mouth[starting_index:, :].reshape(40-starting_index*2)


def normalize_dataset(landmarks_array, starting_index):
    new_landmarks_array = np.zeros((landmarks_array.shape[0], (landmarks_array.shape[1] - starting_index*2)))
    for i in range(landmarks_array.shape[0]):
        new_landmarks_array[i] = get_mouth_normalized(landmarks_array[i], starting_index)
    return new_landmarks_array


if __name__ == '__main__':
    landmarks_array = pickle.load(open(sys.argv[1], 'rb'))
    print(landmarks_array.shape)
    pickle.dump(normalize_dataset(landmarks_array, 12), open('landmarks_array_normalized.pkl', 'wb'))
