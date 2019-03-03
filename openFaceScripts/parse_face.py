from __future__ import division, print_function, unicode_literals
import cv2
import numpy as np


def read_one_face(inpFile):
    new_face = []
    if inpFile.readline().startswith("face"):
        inpFile.readline()
        for i in range(68):
            new_string = str(inpFile.readline())
            new_face.append([int(val) for val in new_string.split()])
        inpFile.readline()
    else:
        return None
    return new_face


def read_file(inpFile):
    faces = []
    new_face = read_one_face(inpFile)
    while(new_face is not None):
        faces.append(new_face)
        new_face = read_one_face(inpFile)
    return faces


def cal_mean_distance(face, new_face):
    return np.sum(np.array(np.array(face) - np.array(new_face))**2)/68.0


def draw_face(img, face):
    for point in face:
        cv2.circle(img, tuple(point), 3, [0, 0, 255], 3)
