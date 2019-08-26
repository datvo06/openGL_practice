from __future__ import print_function, unicode_literals, division
import os
import sys
import shutil


if not os.path.exists('gathered'):
    os.makedirs('gathered')


if __name__ == '__main__':
    i = 1
    index_file = 0
    while(os.path.exists(str(i))):
        for root, dirs, files in os.walk(str(i), topdown=False):
           for fname in files:
               if fname.endswith(".jpg"):
                   fname_no_ext = os.path.splitext(fname)[0]
                   fname_pts = fname_no_ext + ".pts"
                   imgpath = os.path.join(root, fname)
                   ptspath = os.path.join(root, fname_pts)
                   shutil.copy(imgpath, os.path.join('gathered', str(index_file) + ".jpg"))
                   shutil.copy(ptspath, os.path.join('gathered', str(index_file) + ".pts"))
                   index_file += 1
        i += 1
