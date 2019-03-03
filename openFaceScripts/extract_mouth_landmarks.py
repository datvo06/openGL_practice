from __future__ import division, print_function, unicode_literals
import os
import glob
import sys
from parse_face import read_file
import numpy as np
import pickle
from sklearn.cluster import KMeans


if __name__ == '__main__':
    print("Sys argv: ", sys.argv)
    list_files = list(glob.glob(os.path.join(sys.argv[1], '*.pts')))
    pickle.dump(list_files, open('list_file.pkl', 'wb'))
    mouths_landmark = []
    for file_path in list_files:
        face = np.array(read_file(open(file_path, 'r'))[0])
        # Extract mouth region
        mouth_landmark = face[48:]
        mouths_landmark.append(mouth_landmark)
    mouths_landmark = np.array(mouths_landmark)
    mouths_landmark = mouths_landmark.reshape(
        (mouths_landmark.shape[0],
         mouths_landmark.shape[1]*mouths_landmark.shape[2]))
    print("Mouth land mark total shape: ", mouths_landmark.shape)
    kmeans_cluster = KMeans(n_clusters=10)
    labels = kmeans_cluster.fit_predict(mouths_landmark)
    centers = kmeans_cluster.cluster_centers_.astype("uint8")
    pickle.dump(mouths_landmark, open('land_marks.pkl', 'wb'))
    pickle.dump(labels, open('clustered_labels.pkl', 'wb'))
    pickle.dump(centers, open('centers.pkl', 'wb'))
