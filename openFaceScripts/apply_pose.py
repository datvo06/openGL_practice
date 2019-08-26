# From Python
# It requires OpenCV installed for Python
import sys
import cv2
import os
from sys import platform
import glob

# Remember to add your installation path here
# Option a
dir_path = os.path.dirname(os.path.realpath(__file__))
if platform == "win32": sys.path.append(dir_path + '/../../python/openpose/');
else: sys.path.append('../../python');
# Option b
# If you run `make install` (default path is `/usr/local/python` for Ubuntu), you can also access the OpenPose/python module from there. This will install OpenPose and the python library at your desired installation path. Ensure that this is in your python path in order to use it.
# sys.path.append('/usr/local/python')

# Parameters for OpenPose. Take a look at C++ OpenPose example for meaning of components. Ensure all below are filled
try:
    from openpose import *
except:
    raise Exception('Error: OpenPose library could not be found. Did you enable `BUILD_PYTHON` in CMake and have this Python script in the right folder?')
params = dict()
params["logging_level"] = 3
params["output_resolution"] = "-1x-1"
params["net_resolution"] = "-1x368"
params["model_pose"] = "BODY_25"
params["alpha_pose"] = 0.6
params["scale_gap"] = 0.3
params["scale_number"] = 1
params["hand"] = True
params["face"] = True
params["render_threshold"] = 0.05
# If GPU version is built, and multiple GPUs are available, set the ID here
params["num_gpu_start"] = 0
params["disable_blending"] = False
# Ensure you point to the correct path where models are located
params["default_model_folder"] = dir_path + "/../../../models/"
# Construct OpenPose object allocates GPU memory
openpose = OpenPose(params)


def merge_training_set(in_dir, out_dir, index):
    file_paths = glob.glob(in_dir + '/*.jpg')
    filenames_full = list(
        [os.path.split(file_path)[1]
         for file_path in file_paths])
    filenames = list(
        [os.path.splitext(filename_full)[0]
         for filename_full in filenames_full])
    framelists = list(
        sorted(int(filename) for filename in filenames))
    for count, i in enumerate(framelists):

        img = cv2.imread(os.path.join(in_dir, str(i) + ".jpg"))
        keypoints, output_image = openpose.forward(img, True)
        cv2.imwrite(os.path.join(out_dir,
                                 str(index+count) + ".jpg"), output_image)
    index += len(framelists)
    return index


if __name__ == '__main__':
    if not os.path.exists('training'):
        os.makedirs('training')
    index = 0
    for path in sys.argv[1:]:
        out_path = 'training'
        if not (os.path.exists(out_path)):
            os.makedirs(out_path)
        index = merge_training_set(path, out_path, index)
