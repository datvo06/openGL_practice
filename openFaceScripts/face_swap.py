''' 3d morphable model example
3dmm parameters --> mesh
fitting: 2d image + 3dmm -> 3d face
'''
import os, sys
import subprocess
import numpy as np
import scipy.io as sio
from skimage import io
from time import time
import matplotlib.pyplot as plt
import cv2
from auto_script import read_whole_csv
import csv

import face3d
import cPickle as pickle
from face3d import mesh
from face3d.morphable_model import MorphabelModel

# --------------------- Forward: parameters(shape, expression, pose) --> 3D obj --> 2D image  ---------------
# --- 1. load model
bfm = MorphabelModel('/home/theengineer/Marc/openGL_practice/syntethic_data/model/BFM.mat')
print('init bfm model success')

# --- 2. generate face mesh: zero first
sp = bfm.get_shape_para('zero')
ep = bfm.get_exp_para('zero')
vertices = bfm.generate_vertices(sp, ep)

# --- 3. transform vertices to proper position
s = 8e-04
angles = [10, 30, 20]
t = [0, 0, 0]
transformed_vertices = bfm.transform(vertices, s, angles, t)
projected_vertices = transformed_vertices.copy() # using stantard camera & orth projection

# Let's open the csv file
in_video_path = sys.argv[1]
video_filename_full = os.path.split(in_video_path)[1]
video_filename, video_ext = os.path.splitext(video_filename_full)
csv_file_path = os.path.join('./processed', video_filename + '.csv')
csv_file = open(csv_file_path, 'r')
reader = csv.reader(csv_file)
faces = read_whole_csv(reader, True, 1000)
csv_file.close()
csv_file = open(csv_file_path, 'r')
reader = csv.reader(csv_file)
faces2d = read_whole_csv(reader, False, 1000)
csv_file.close()
print(len(faces))
target_shape = pickle.load(open(sys.argv[2], 'rb'))
target_colors = pickle.load(open(sys.argv[3], 'rb'))
h = 512; w =512

if not (os.path.exists('fitted')):
    os.makedirs('fitted')
save_folder = 'fitted'
# Let's open the cap
cap = cv2.VideoCapture(in_video_path)
faces2d = np.array(faces2d)

for i, face in enumerate(faces):
    _, frame = cap.read()
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    print(i)
    # Let's fit
    ## only use 68 key points to fit
    x = np.array(face).reshape(68, 3)[:, :2]
    X_ind = bfm.kpt_ind # index of keypoints in 3DMM. fixed.
    # fit
    fitted_sp, fitted_ep, fitted_s, fitted_angles, fitted_t = bfm.fit(x, X_ind, max_iter = 3)

    # combine fitted_ep with target_shape
    fitted_vertices = bfm.generate_vertices(target_shape, fitted_ep)
    transformed_vertices = bfm.transform(fitted_vertices, fitted_s, fitted_angles, fitted_t)

    image_vertices = mesh.transform.to_image(transformed_vertices, h, w)
    fitted_image = mesh.render.render_colors(image_vertices, bfm.triangles, target_colors, h, w)

    fitted_image[fitted_image < -1.0] = -1.0
    fitted_image[fitted_image > 1.0] = 1.0
    io.imsave('{}/{}.jpg'.format(save_folder, i+1), fitted_image)
