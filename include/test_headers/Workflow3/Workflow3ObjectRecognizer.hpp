#ifndef __Workflow3_HPP
#define __Workflow3_HPP
#include <customized_utils/image_utils.hpp>
#include <customized_utils/general_utils.hpp>
#include <customized_utils/LinAlgUtils/linalg_utils.hpp>
#include <customized_utils/graphics_utils.hpp>


/**
 * @brief Hai's object recognition method
 */
void _recognizeObjectWorkflow3(cv::Mat& image, cv::Mat& objectMat);
void _recognizeObjectWorkflow3Graphics(cv::Mat& image, cv::Mat& objectMat);

#endif
