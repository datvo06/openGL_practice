//
//  image_utils.cpp
//  TestIphoneCV
//
//  Created by Dat Nguyen Thanh on 12/9/17.
//  Copyright © 2017 Dat Nguyen Thanh. All rights reserved.
//

#include <customized_utils/image_utils.hpp>
#include <vector>
#include <math.h>


std::vector<cv::Mat> DatCustom::ImUtil::getChannels(cv::Mat inputImage, int* pNumChannel){
	*pNumChannel = (int)fmin(inputImage.channels(), 3);
	std::vector<cv::Mat> planes(*pNumChannel);
  for (int i = 0; i < 3; i++)
        cv::extractChannel(cv::_InputArray(inputImage), cv::_OutputArray(planes[i]), i);
	return planes;
}


double DatCustom::ImUtil::fastMedianMat(cv::Mat input, int nVals){
    //Compute histogram of single channel matrix
    float range[] = {0, (float)nVals};
    const float* histRange = {range};
    bool uniform = true; bool accumulate = false;
    cv::Mat hist;
    cv::calcHist(&input, 1, 0, cv::_InputArray(cv::Mat()), cv::_OutputArray(hist), 1, &nVals, &histRange, uniform, accumulate);
    
    //Compute the cumulative distribution function (CDF)
    cv::Mat cdf;
    hist.copyTo(cv::_OutputArray(cdf));
    for(int i = 1; i <= nVals-1; i++){
        cdf.at<float>(i) += cdf.at<float>(i-1);
    }
    cdf /= input.total();
    
    //Compute median
    double medianVal = 0.0f;
    for(int i=0; i <= nVals-1; i++) if(cdf.at<float>(i) >= 0.5) {medianVal = i; break;}
    return medianVal;
}


cv::Mat DatCustom::ImUtil::tempKmeans(const cv::Mat& src, int clusterCount)
{
    cv::Mat samples(src.rows * src.cols, 3, CV_32F);
    for( int y = 0; y < src.rows; y++ )
        for( int x = 0; x < src.cols; x++ )
            for( int z = 0; z < 3; z++)
                samples.at<float>(y + x*src.rows, z) = src.at<cv::Vec3b>(y,x)[z];
    
    
    cv::Mat labels;
    int attempts = 15;
    cv::Mat centers;
    cv::kmeans(samples, clusterCount, labels, cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, cv::KMEANS_PP_CENTERS, centers );
    
    
    cv::Mat new_image( cv::Size(src.rows, src.cols), CV_8U);
    for( int y = 0; y < src.rows; y++ )
        for( int x = 0; x < src.cols; x++ )
        {
            int cluster_idx = labels.at<int>(y+ x*src.rows,0);
            new_image.at<int>(y,x) = cluster_idx;
        }
    return new_image;
}


cv::Mat DatCustom::ImUtil::rotateBound(const cv::Mat& rImage, float angle){
    //Find dimmension of the image
    int height = rImage.rows; int width = rImage.cols;
    float cX = sqrt(width);  float cY = sqrt(height);
    // grab the rotation matrix (by applying the negative of the angle to rotate clockwise)
    // then grab the sine and cosine
    cv::Mat M = cv::getRotationMatrix2D(cv::Point2f(cX, cY), -angle, -1.0);
    double cos_val = fabs(M.at<double>(0, 0));
    double sin_val = fabs(M.at<double>(0, 1));
    
    // compute the new bounding dimmensions of the image
    int nW = (int)((height*sin_val)+(width*cos_val));
    int nH = (int)((height*cos_val)+(width*sin_val));
    
    cv::Mat rDest(rImage.size(), rImage.type());
    // Adjust the rotation matrix to take into account translation
    M.at<double>(0, 2) += ((nW/2.0) - cX);
    M.at<double>(1, 2) += ((nH/2.0) - cY);
    
    cv::warpAffine(rImage, rDest, M, cv::Size(nW, nH));
    return rDest;
}



std::vector<cv::Point>order_points(std::vector<cv::Point>c) {
    std::vector<cv::Point> result;
    cv::Point tl = c[0], bl, tr, br = c[0];
    int indextl = 0, indexbr = 0, i, j;
    for (i= 0; i < (int)c.size(); i++) {
        if (tl.x + tl.y > c[i].x+c[i].y) {
            tl = c[i];
            indextl = i;
        }
        if (br.x + br.y < c[i].x + c[i].y) {
            br = c[i];
            indexbr = i;
        }
    }
    for (i = 0; i < (int)c.size(); i++) {
        if (i != indexbr&&i != indextl) {
            tr = c[i];
            for (j = i+1; j < (int)c.size(); j++) {
                if (j != indexbr&&j != indextl) {
                    bl = c[j];
                    break;
                }
            }
            break;
        }
    }
    result.push_back(tl);
    result.push_back(tr);
    result.push_back(br);
    result.push_back(bl);
    
    return result;
}


cv::Mat DatCustom::ImUtil::fourPointsTransform(const cv::Mat& image, const std::vector<cv::Point>& c) {
    std::vector<cv::Point> rect = order_points(c);
    cv::Mat result;
    cv::Point tl, tr, br, bl;
    tl = rect[0];
    tr = rect[1];
    br = rect[2];
    bl = rect[3];
    int widthA = floor(sqrt((tl.x - tr.x)*(tl.x - tr.x) + (tl.y - tr.y)*(tl.y - tr.y)));
    int widthB = floor(sqrt((bl.x - br.x)*(bl.x - br.x) + (bl.y - br.y)*(bl.y - br.y)));
    int heightA = floor(sqrt((tl.x - bl.x)*(tl.x - bl.x) + (tl.y - bl.y)*(tl.y - bl.y)));
    int heightB = floor(sqrt((tr.x - br.x)*(tr.x - br.x) + (tr.y - br.y)*(tr.y - br.y)));
    int maxWidth = widthA;
    if (maxWidth < widthB)
        maxWidth = widthB;
    int maxHeight = heightA;
    if (maxHeight < heightB)
        maxHeight= heightB;
    
    std::vector<cv::Point> dst;
    dst.push_back(cv::Point(0, 0));
    dst.push_back(cv::Point(maxWidth-1, 0));
    dst.push_back(cv::Point(maxWidth - 1, maxHeight-1));
    dst.push_back(cv::Point(0, maxHeight - 1));
    cv::Mat M, warped;
    cv::Size sizex(maxWidth, maxHeight);
    //M = getPerspectiveTransform(rect, dst);
    cv::Point2f source_points[4];
    cv::Point2f dest_points[4];
    cv::Mat _transform_matrix;
    
    source_points[0] = rect[0];
    source_points[1] = rect[1];
    source_points[2] = rect[2];
    source_points[3] = rect[3];
    
    dest_points[0] = cv::Point(0, 0);
    dest_points[1] = cv::Point(maxWidth - 1, 0);
    dest_points[2] = cv::Point(maxWidth - 1, maxHeight - 1);
    dest_points[3] = cv::Point(0, maxHeight - 1);
    
    cv::Mat dstx = image.clone();
    _transform_matrix = cv::getPerspectiveTransform(source_points, dest_points);
    cv::warpPerspective(image, dstx, _transform_matrix, sizex);
    
    return dstx;
}
