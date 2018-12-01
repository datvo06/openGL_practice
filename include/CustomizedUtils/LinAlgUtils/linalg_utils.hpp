//
//  linalg_utils.hpp
//  TestIphoneCV
//
//  Created by Dat Nguyen Thanh on 23/3/18.
//  Copyright © 2017 Dat Nguyen Thanh. All rights reserved.
//

#ifndef __DAT_LINALG_HPP
#define __DAT_LINALG_HPP

#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
#include <opencv2/opencv.hpp>


enum distance_type {EUCLIDEAN, COSINE};


namespace DatCustom{
	namespace ConvertUtil{
		/**
		 * @brief Converting opencv matrix to armadillo's matrix, change from row major to column major. Note: single channel
		 * @param inputMat The opencv matrix, row-major
		 */
		arma::mat fromCVMat(const cv::Mat& inputMat);
		/**
		 * @brief Converting from armadillo matrix to opencv matrix, change from column-major to row major. Note: single channel
		 * @param inputMat The armadillo matrix, column-major
		 */
		cv::Mat fromArmaMat(const arma::mat& inputMat);
		/**
		 * @brief flattenImage (h, w ,nChannels) to armadillo (nChannels, h*w)
		 * @param img : input image, opencv
		 * @param debug : True if you want to print stuff out
		 */
		arma::mat flattenImageToArmaMat(cv::Mat img, bool debug=false);
	}
	namespace ProbUtil{
		/**
		 * @brief pick an index in (0, probVec.size()-1)
		 * @param probVec the probability vector (weights)
		 * @param cutoff Will not take any index with value below this
		 * @param debug print stuffs
		 */
		int weightedProbRandomPick(const arma::mat& probVec, double cutoff=0.001, bool debug=false);
		/**
		 * @brief pick an index in (0, probVec.size()-1) using cumulative sum, easy on memory
		 * @param probVec the probability vector (weights)
		 * @param cutoff Will not take any index with value below this
		 * @param debug print stuffs
		 */
		int weightedProbRandomPickCumSum(const arma::mat& probVec, bool debug=false);
	}
	namespace LinAlgUtil{
		/**
		 * @brief calculate distance between vectors, equivalent to numpy cdist
		 * @param listVectors std::vector of vectors
		 */
		arma::mat cdist(arma::mat inputVecs1,
				arma::mat inpuVecs2,
				distance_type theType=EUCLIDEAN);
	}
	namespace ClusterUtil{
		/**
		 * @brief perform kmeans using armadillo
		 * @param img : grayscale or colored image (maximum 3-dimensional)
		 * @param numClusters : number of clusters
		 * @param pLabelMat : label mat storage, NULL = nostore
		 * @param debug : print stuffs
		 */
		cv::Mat imageKmeans(cv::Mat img, int numClusters, arma::mat* pLabelMat=NULL, bool debug=false);
	/**
		 * @brief perform kmeans using armadillo
		 * @param img : grayscale or colored image (maximum 3-dimensional)
		 * @param numClusters : number of clusters
		 * @param batchSize : batch size
		 * @param pLabelMat : label mat storage, NULL = nostore
		 * @param debug : print stuffs
		 */
		cv::Mat imageMiniBatchKmeans(cv::Mat img, int numClusters, int batchSize=100, arma::mat* pLabelMat=NULL, bool debug=false);
	}
}
#endif
