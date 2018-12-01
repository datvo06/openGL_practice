#ifndef __Workflow2_HPP
#define __Workflow2_HPP
#include <customized_utils/image_utils.hpp>
#include <customized_utils/general_utils.hpp>
#include <customized_utils/LinAlgUtils/linalg_utils.hpp>


cv::Mat customizedProcessWithDistanceWorkflow2(const cv::Mat& median_image_hor, double* pDistance);
cv::Mat furtherProcessWarpedWorkflow2(const cv::Mat& warped);
void _recognizeObjectWorkflow2(cv::Mat& image, cv::Mat& objectMat);

#endif
