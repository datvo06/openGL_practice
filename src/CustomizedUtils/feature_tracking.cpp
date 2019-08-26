#include <customized_utils/feature_tracking.hpp>
#include <customized_utils/general_utils.hpp>
#include <customized_utils/LinAlgUtils/linalg_utils.hpp>
#include <utility>
#include <memory>
#include <stdlib.h>


using DatCustom::ImUtil::PointPair;
using std::pair;
using std::unique_ptr;
using std::make_pair;

static cv::Size divideSizeByFactor(int oldW, int oldH, float factor){
	return cv::Size(oldW/factor, oldH/factor);
}


static void calculateGradient(cv::Mat img, pair<cv::Mat, cv::Mat>& gradPairOutput){
	cv::Sobel(img, gradPairOutput.first, CV_64F, 1, 0, 3);
	cv::Sobel(img, gradPairOutput.second, CV_64F, 0, 1, 3);
}


static void resizePairByFactor(pair<cv::Mat, cv::Mat>* prevLayer, pair<cv::Mat, cv::Mat>*  thisLayer, int factor){
	cv::Mat& prevLayerImg1 = prevLayer->first;
		cv::Mat& prevLayerImg2 = prevLayer->second;
		cv::resize(
				prevLayerImg1, thisLayer->first,
				divideSizeByFactor(prevLayerImg1.size[0], prevLayerImg1.size[1], factor)
		);
		cv::resize(prevLayerImg2, thisLayer->second,
				divideSizeByFactor(prevLayerImg2.size[0], prevLayerImg2.size[1], factor)
		);
}


int comparPoint (const void* p1, const void* p2){
	cv::Point2i* pPoint1 = (cv::Point2i*)p1;
	cv::Point2i* pPoint2 = (cv::Point2i*)p2;
	if (pPoint1->x < pPoint2->x){
		return -1;
	}
	else if(pPoint1->x == pPoint2->x){
		if (pPoint1->y < pPoint2->y){
			return -1;
		}
		else if (pPoint1->y == pPoint2->y) return 0;
		else return 1;
	}
	else return 1;
}


static unique_ptr<std::vector<cv::Point2i>> getSetUniquePoint(const cv::Point2i* points, size_t arraySize){
	unique_ptr<std::vector<cv::Point2i>> finalSet;
	// 1. sort array by x, then y
	qsort((void*)points, arraySize, sizeof(cv::Point2i), comparPoint);
	// Insert unique set
	finalSet->push_back(points[0]);
	for (size_t i = 1; i < arraySize; i++){
		if (finalSet->back().x == points[i].x && finalSet->back().y == points[i].y)
			continue;
		finalSet->push_back(points[i]);
	}
	return finalSet; 						// C++ auto std::move at this point for us.
}


static unique_ptr<std::vector<cv::Point2i>> trackKLTSingleLayer(pair<cv::Mat, cv::Mat>& grayImgPair,
		pair<cv::Mat, cv::Mat>& firstImgGrad,
		const std::vector<cv::Point2i>& features,
		const std::vector<cv::Point2i>& initialGuess){
	/// Start iteration
	/// 1. For every feature, compute matrix G
	/// 2. And disparity map b
	/// 3. Update
	/// 4. Check cond to stop
}	


std::vector<PointPair> DatCustom::ImUtil::KLTPointTracker::trackFeatures(cv::Mat img1, cv::Mat img2,
	 	const std::vector<cv::Point2i>& features, void* pyramidLevel){
	// KLT only track grayscale
	cv::Mat gray1, gray2;
	cv::cvtColor(img1, gray1, cv::COLOR_BGR2GRAY);
	cv::cvtColor(img2, gray2, cv::COLOR_BGR2GRAY);
	std::vector<PointPair> featurePair;
	std::vector<std::vector<cv::Point2i>> pyramidFeaturePoint;
	unique_ptr<std::vector<pair<cv::Mat, cv::Mat>>> pyramidImagePairs(new std::vector<pair<cv::Mat, cv::Mat>>());
	unique_ptr<std::vector<pair<cv::Mat, cv::Mat>>> gradientPyramid(new std::vector<pair<cv::Mat, cv::Mat>>);

	int L = *(int*) pyramidLevel;
	/// 1.0. Build pyramid image layer
	/// 1.1. Map old features to pyramid level
	/// 1.2. Calculate gradient pyramid
	pyramidFeaturePoint.push_back(features);
	pyramidImagePairs->push_back(make_pair(gray1, gray2));
	gradientPyramid->push_back(make_pair(cv::Mat(), cv::Mat()));
	calculateGradient(gray1, gradientPyramid->back());


	for (int i = 1; i < L; i++){
		pyramidFeaturePoint.push_back(std::vector<cv::Point2i>());
		for (auto it = pyramidFeaturePoint[i-1].begin(); it != pyramidFeaturePoint[i-1].end(); i++){
			pyramidFeaturePoint.back().push_back(cv::Point2i(it->x/2, it->y/2));
		}
		pyramidImagePairs->push_back(make_pair(cv::Mat(), cv::Mat()));
		resizePairByFactor(&pyramidImagePairs->at(i-1), &pyramidImagePairs->at(i), 2);
		calculateGradient(pyramidImagePairs->back().first, gradientPyramid->back());
	}
	// 2. perform KLT Track on pyramid level
	std::vector<cv::Point2i> prevEstimation;
	for (int i = L-1; i >= 0; i--){
		/// First step: get unique set of point at this layer, since shrinking will make some points have save estimation
		unique_ptr<std::vector<cv::Point2i>> uniquePoints = getSetUniquePoint(pyramidFeaturePoint[i].data(), pyramidFeaturePoint[i].size());
		/// Second step: optimize them!!!

		std::vector<cv::Point2i> uniquePointSet;
		cv::Point2i g; 													/// Read the paper, g is the prev estimation
		if ( i == L - 1) {
			// Initialize the first 
			g.x = 0; g.y = 0;
		}
		else{
		}
	}
	// starts from the smallest level
	// 3. Get them to a pair
	throw DatCustom::Exception::NotImplementedException();
	return featurePair;
}
