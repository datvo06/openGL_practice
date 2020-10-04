from __future__ import absolute_import, print_function, unicode_literals
import numpy as np
import cv2
import sys
import os


fourcc = cv2.VideoWriter_fourcc('X', 'V', 'I', 'D')
zero_original_frame = cv2.imread(os.path.join(
    sys.argv[1], '1.jpg'))
out = cv2.VideoWriter(
    'output_visualize_nick_3ddfa.avi', fourcc, 20.0,
    (int(zero_original_frame.shape[1] + zero_original_frame.shape[0]*8.0/3.0),
    zero_original_frame.shape[0]))
for i in range(1, 1000):
    zero_original_frame = cv2.imread(
        os.path.join(sys.argv[1], str(i) + '.jpg'))
    # zero_data = cv2.imread('fitted/' + str(i) + '.jpg')
    zero_data = cv2.imread(os.path.join(sys.argv[2], 'frame_' + str(i) + '.jpg'))
    zero_data2 = cv2.imread(os.path.join(sys.argv[3], 'frame_' + str(i) + '.jpg'))
    zero_data_resized = cv2.resize(zero_data,
                                   (int(zero_original_frame.shape[0]*4.0/3.0),
                                   zero_original_frame.shape[0]),
                                   interpolation = cv2.INTER_AREA)
    zero_data2_resized = cv2.resize(zero_data2,
                                   (int(zero_original_frame.shape[0]*4.0/3.0),
                                   zero_original_frame.shape[0]),
                                   interpolation = cv2.INTER_AREA)
    new_frame = np.zeros(
        (zero_original_frame.shape[0],
         int(zero_original_frame.shape[1] + zero_original_frame.shape[0]*8.0/3.0), 3)).astype(np.uint8)
    new_frame[:, -int(zero_original_frame.shape[0]*8.0/3.0):
              -int(zero_original_frame.shape[0]*4.0/3.0)] = cv2.flip(zero_data_resized, 0)
    new_frame[:, -int(zero_original_frame.shape[0]*4.0/3.0):] = cv2.flip(zero_data2_resized, 0)
    new_frame[:, :int(zero_original_frame.shape[1])] = zero_original_frame
    out.write(new_frame)
    cv2.imshow('frame', new_frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

print("Here")
out.release()
