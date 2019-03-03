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

tp = bfm.get_tex_para('zero')
colors = bfm.generate_colors(tp)
colors = np.minimum(np.maximum(colors, 0), 1)

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
h = 512; w =512

if not (os.path.exists('fitted')):
    os.makedirs('fitted')
save_folder = 'fitted'
# For textures
# First, let's stack up 199 textures components
D_pc = bfm.model['texPC']                   # (Nx3)xD
D_pc_transposed = np.transpose(D_pc)        # D x (Nx3)
lhs_matrix = np.matmul(D_pc_transposed, D_pc) # D x D
inv_equation = np.matmul(np.linalg.inv(lhs_matrix), D_pc_transposed)
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

    # verify fitted parameters
    fitted_vertices = bfm.generate_vertices(fitted_sp, fitted_ep)
    transformed_vertices = bfm.transform(fitted_vertices, fitted_s, fitted_angles, fitted_t)

    image_vertices = mesh.transform.to_image(transformed_vertices, h, w)
    # Let's calculate an affine transform from the original image to this
    if i == 0:
        image_vertices_kpts = image_vertices[X_ind]
        M, mask = cv2.findHomography(faces2d[i].astype('float32'),
                            image_vertices_kpts[:, :2].astype('float32'))
        def draw_face(img, face2d):
            for point in face2d:
                cv2.circle(img, tuple((int(point[0]), int(point[1]))), 3, [0, 0, 255], 3)
        # draw_face(frame, faces2d[i])
        dst = cv2.warpPerspective(frame, M, (w, h))
        # draw_face(dst, image_vertices_kpts[:, :2])
        image_vertices_copy = image_vertices[:, [1, 0, 2]]
        textures = dst[
            image_vertices_copy[:, 0].astype('uint'),
            image_vertices_copy[:, 1].astype('uint'),
            :
        ].astype('float32')
        '''
        textures = textures.reshape((textures.shape[0]*3, 1))
        textures = textures.astype('float32')
        # Let's fit !!!
        DeltaColor = textures - bfm.model['texMU']  # (Nx3)x1
        texParams = np.matmul(inv_equation, DeltaColor) # D x 1
        print(texParams.shape)
        print("Final norm: ", np.linalg.norm(np.matmul(bfm.model['texPC'], texParams) - bfm.model['texMU']))
        colors_fit = bfm.generate_colors(texParams)
        '''
        colors_fit = textures/255.0
        pickle.dump(colors_fit, open('fitted/colors.pkl', 'wb'))
        pickle.dump(fitted_sp, open('fitted/sp.pkl', 'wb'))
    fitted_image = mesh.render.render_colors(image_vertices, bfm.triangles, colors_fit, h, w)


    fitted_image[fitted_image < -1.0] = -1.0
    fitted_image[fitted_image > 1.0] = 1.0
    io.imsave('{}/{}.jpg'.format(save_folder, i+1), fitted_image)
    io.imsave('{}/{}_perspectived.jpg'.format(save_folder, i+1), dst)
