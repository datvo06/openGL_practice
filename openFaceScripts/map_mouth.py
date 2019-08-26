import cv2
import os
import sys
import numpy as np
from scipy.spatial.distance import cdist
import pickle


if __name__ == '__main__':
    landmarks_train = pickle.load(open('./landmarks_array_normalized_train.pkl', 'rb'))
    landmarks_target = pickle.load(open('./landmarks_array_normalized.pkl', 'rb'))
    index_source = []
    for landmark_target in landmarks_target:
        distances = cdist(np.array([landmark_target]), landmarks_train[:, :], metric='euclidean')
        index_source.append(np.argmin(distances))
    pickle.dump(index_source, open('map.pkl', 'wb'))
