from __future__ import absolute_import, print_function, unicode_literals
import numpy as np
import cv2
import os


fourcc = cv2.VideoWriter_fourcc(*'XVID')
zero_original_frame = cv2.imread('./obamaFrames/1.jpg')
out = cv2.VideoWriter('output_visualize.avi', fourcc, 20.0,
                      (int(zero_original_frame.shape[0]*8.0/3.0),
                       zero_original_frame.shape[0]))
for i in range(1, 1000):
    zero_original_frame = cv2.imread('./obamaFrames/' + str(i) + '.jpg')
    zero_data = cv2.imread('./data/frame_' + str(i) + '.jpg')
    zero_data_resized = cv2.resize(zero_data, (int(zero_original_frame.shape[0]/600.0*800),
                                               zero_original_frame.shape[0]),
                                   interpolation = cv2.INTER_AREA)
    new_frame = np.zeros((zero_original_frame.shape[0],
                          int(zero_original_frame.shape[0]*8.0/3.0), 3)).astype(np.uint8)
    new_frame[:, -int(zero_original_frame.shape[0]/600*800):] = cv2.flip(zero_data_resized, 0)
    new_frame[:, :int(zero_original_frame.shape[1])] = zero_original_frame
    out.write(new_frame)
    cv2.imshow('frame', new_frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

print("Here")
out.release()
