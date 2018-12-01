#include <customized_utils/feature_finding.hpp>


using namespace DatCustom;


std::vector<cv::Point2i> ImUtil::CVHarrisCornderDetection::detectFeatures(cv::Mat img, void* params){
	cv::Mat dst = cv::Mat::zeros( img.size(), CV_32FC1 );
	cv::Mat dstNorm, dstNormScaled;
	cv::Mat srcGray;
	if (img.channels() > 1)
		cv::cvtColor(img, srcGray, cv::COLOR_BGR2GRAY);
	else
		img.copyTo(srcGray);

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;
	int thresh = 200;
	int max_thresh = 255;

  /// Detecting corners
  cornerHarris( srcGray, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT );
	cv::normalize(dst, dstNorm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
	std::vector<cv::Point> features;
	for( int j = 0; j < dstNorm.rows ; j++ ) {
	 	for( int i = 0; i < dstNorm.cols; i++ )
          {
            if((int)dstNorm.at<float>(i, j) > thresh)
							features.push_back(cv::Point2i(i, j));
          }
     }
}
