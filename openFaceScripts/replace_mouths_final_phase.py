from __future__ import print_function, unicode_literals, division
import os
import sys
import cv2
fourcc = cv2.VideoWriter_fourcc(*'MJPG')


if __name__ == '__main__':
    writer = cv2.VideoWriter('replaced_video.avi', fourcc, 15.0, (512, 256))
    cap = cv2.VideoCapture(sys.argv[1])
    if not os.path.exists("recomprised_frames"):
        os.makedirs("recomprised_frames")
    i = 0
    while(cap.isOpened()):
        try:
            ret, frame = cap.read()
            img_path = os.path.join('final_dir', str(i) + '.jpg')
            frame[:, 256:] = cv2.imread(img_path)
            print(" Frame number : ", str(i))
            writer.write(frame)
            i += 1
        except:
            break

cap.release()
cv2.destroyAllWindows()
