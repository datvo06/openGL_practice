//
// feature_fiding.hpp
// 3D reconstruction practice
//
// Created by Dat Nguyen Thanh on 7/15/2018.
// Copyright © 2017 Dat Nguyen Thanh. All rights reserved.
//

#ifndef __CUSTOMIZED_IMAGE_UTIL_FIND_FEATURE_HPP__
#define __CUSTOMIZED_IMAGE_UTIL_FIND_FEATURE_HPP__
#include <vector>
#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#endif
namespace DatCustom{
	namespace ImUtil{
		/*! A general Point feature finding interface */
		class PointFeatureFinding{
			public:
				PointFeatureFinding() {};
				/**
				 * @brief detecting features, returning a vector of point(x, y)
				 * @param img an opencv image, grayscale or colored
				 */
				virtual std::vector<cv::Point2i> detectFeatures(cv::Mat img, void* params) = 0;
		};

		/*! Custom implementation of feature finding interface, implementing harris cornder detection */
		class HarrisCornerDetection: public PointFeatureFinding{
			public:
				HarrisCornerDetection() {};
				std::vector<cv::Point2i> detectFeatures(cv::Mat img, void* params) override;
		};

		/*! Implement Feature finding interface using opencv implementation of Harris corner detection */
		class CVHarrisCornderDetection: public PointFeatureFinding{
			public:
				CVHarrisCornderDetection() {};
				/**
				 * @brief using opencv implementation of harris corner detector
				 * @param img The input image
				 * @param params structure (int blockSize, int apertureSize, double k, int thresh, int max_thresh)
				 */
				std::vector<cv::Point2i> detectFeatures(cv::Mat img, void* params) override;
		};
	}
}
#endif
