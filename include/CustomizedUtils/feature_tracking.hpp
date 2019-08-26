//
// feature_tracking.hpp
// 3D reconstruction practice
//
// Created by Dat Nguyen Thanh on 7/15/2018.
// Copyright © 2017 Dat Nguyen Thanh. All rights reserved.
//

#ifndef __CUSTOMIZED_IMAGE_UTIL_TRACK_FEATURE_HPP__
#define __CUSTOMIZED_IMAGE_UTIL_TRACK_FEATURE_HPP__
#include <vector>
#include <utility>
#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#endif

namespace DatCustom{
	namespace ImUtil{
		typedef std::pair<cv::Point2i, cv::Point2i> PointPair;
		/*! A general point feature tracking interface between 2 image */
		class PointFeatureTracker2Img{
			public:
				PointFeatureTracker2Img() {};
				/**
				 * @brief given features in from the first image, track them on 2nd image
				 * @param img1 the first image which contains the features
				 * @param img2 the second image to track features
				 * @param features a vector of point(x, y) features
				 * @param params other parameters based on implementation
				 */
				virtual std::vector<PointPair> trackFeatures(cv::Mat img1, cv::Mat img2, const std::vector<cv::Point2i>& features, void* params);
		};


		/*! Kanade-Lucas-Tomasi Point Tracking */
		class KLTPointTracker: public PointFeatureTracker2Img{
			public:
				KLTPointTracker() {};
				/**
				 * @brief vanilla implementation of cusomized KLT (for point tracking)
				 * @param img1 the first image which contains the features, color
				 * @param img2 the second image to track features, color
				 * @param features a vector of point(x, y) features
				 * @param pyramidLevel real type: int, level of pyramid to map to
				 */
				std::vector<PointPair> trackFeatures(cv::Mat img1, cv::Mat img2, const std::vector<cv::Point2i>& features, void* pyramidLevel) override;
	};
	}
}

#endif
