from __future__ import absolute_import, print_function, unicode_literals
import numpy as np
import cv2
import sys
import os


fourcc = cv2.VideoWriter_fourcc('X', 'V', 'I', 'D')
zero_original_frame = cv2.imread(os.path.join(
    sys.argv[1], '1.jpg'))
n_vids = len(sys.argv) - 2
out = cv2.VideoWriter(
    'output_visualize.avi', fourcc, 20.0,
    (int(zero_original_frame.shape[1] + zero_original_frame.shape[0]*4.0/3.0*n_vids),
    zero_original_frame.shape[0]))
for i in range(1, 1000):
    zero_original_frame = cv2.imread(
        os.path.join(sys.argv[1], str(i) + '.jpg'))
    # zero_data = cv2.imread('fitted/' + str(i) + '.jpg')
    new_frame = np.zeros(
        (zero_original_frame.shape[0],
         int(zero_original_frame.shape[1] +
             zero_original_frame.shape[0]*4.0/3.0*n_vids), 3)).astype(np.uint8)
    new_frame[:, :int(zero_original_frame.shape[1])] = zero_original_frame
    for j in range(2, n_vids+2):
        zero_data = cv2.imread(os.path.join(sys.argv[j], 'frame_' + str(i) + '.jpg'))
        zero_data_resized = cv2.resize(zero_data,
                                    (int(zero_original_frame.shape[0]*4.0/3.0),
                                    zero_original_frame.shape[0]),
                                    interpolation=cv2.INTER_AREA)
        new_frame[:, zero_original_frame.shape[1] + int(zero_original_frame.shape[0]*4.0/3.0*(j-2)):
                zero_original_frame.shape[1] + int(zero_original_frame.shape[0]*4.0/3.0*(j-2))
                  + zero_data_resized.shape[1]
        ] = cv2.flip(zero_data_resized, 0)
    out.write(new_frame)
    cv2.imshow('frame', new_frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

out.release()
