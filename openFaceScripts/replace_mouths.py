from __future__ import print_function, unicode_literals, division
import os
import sys
import cv2


if __name__ == '__main__':
    cap = cv2.VideoCapture(sys.argv[1])
    i = 0
    if not(os.path.exists('cropped_frames')):
        os.makedirs('cropped_frames')
    while(cap.isOpened()):
        try:
            ret, frame = cap.read()
            img_path = os.path.join('cropped_frames', str(i) + '.jpg')
            second_person_frame = frame[:, 256:]
            cv2.imwrite(img_path, second_person_frame)
            i += 1
        except:
            break

cap.release()
cv2.destroyAllWindows()
