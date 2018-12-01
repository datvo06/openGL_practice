#ifndef __Workflow1_HPP
#define __Workflow1_HPP
#include <customized_utils/image_utils.hpp>
#include <customized_utils/general_utils.hpp>


cv::Mat customizedProcessWithDistance(const cv::Mat& median_image_hor, double* pDistance);
cv::Mat furtherProcessWarped(const cv::Mat& warped);
void _recognizeObject(cv::Mat& image, cv::Mat& objectMat);


#endif
