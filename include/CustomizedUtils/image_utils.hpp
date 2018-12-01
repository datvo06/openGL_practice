//
//  image_utils.hpp
//  TestIphoneCV
//
//  Created by Dat Nguyen Thanh on 12/9/17.
//  Copyright © 2017 Dat Nguyen Thanh. All rights reserved.
//

#ifndef image_utils_hpp
#define image_utils_hpp
#include <vector>
#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#endif

#include <CustomizedUtils/feature_finding.hpp>
#include <CustomizedUtils/feature_tracking.hpp>

namespace DatCustom {
    namespace ImUtil{
        /**
         * @brief correctly rotate image by an angle
         * @param rImage : the input image, cv::Mat
         * @param angle : angle, in degree
         */
        cv::Mat rotateBound(const cv::Mat& rImage, float angle);
        /**
         * @brief transform four points to the full screen image
         * @param image : the input image, cv::Mat
         * @param c : the 4 points (std::vector<cv::Point>)
         */
        cv::Mat fourPointsTransform(const cv::Mat& image, const std::vector<cv::Point>& c);
        /**
         * @brief kMeans and return indices
         * @param src : the input image, cv::Mat
         * @param clusterCount : the number of cluster (6 color = 6 cluster )
         */
        cv::Mat tempKmeans(const cv::Mat& src, int clusterCount=6);
				/**
				 * @brief find median value of one-channel Mat
				 * @param input: the Input matrix
				 * @param nVals : number of bins
				 */
				double fastMedianMat(cv::Mat input, int nVals=256);
				/**
				 * @brief split image into 3 channels
				 * @param inputImage the input image
				 * @numChannel number of channel extracted
				 */
				std::vector<cv::Mat> getChannels(cv::Mat inputImage, int* pNumChannel);
				
    }
}

#endif /* image_utils_hpp */
