//
//  linalg_utils.cpp
//  TestIphoneCV
//
//  Created by Dat Nguyen Thanh on 23/3/18.
//  Copyright © 2018 Dat Nguyen Thanh. All rights reserved.
//


#include <customized_utils/LinAlgUtils/linalg_utils.hpp>
#include <customized_utils/image_utils.hpp>
#include <customized_utils/general_utils.hpp>
#include <stdlib.h>


arma::mat DatCustom::ConvertUtil::fromCVMat(const cv::Mat& inputMat){
	cv::Mat transposed;
 	inputMat.convertTo(transposed, CV_64FC1);
	transposed = transposed.t();
	arma::mat armaMat(reinterpret_cast<double*>(transposed.data), inputMat.rows, inputMat.cols);
	return armaMat;
}

cv::Mat DatCustom::ConvertUtil::fromArmaMat(const arma::mat& inputMat){
	return cv::Mat((int)inputMat.n_cols, (int)inputMat.n_rows, CV_64FC1, (void*)inputMat.memptr()).t();
}

arma::mat DatCustom::LinAlgUtil::cdist(arma::mat inputVecs1, arma::mat inputVecs2, distance_type){
	arma::mat results;
	results.zeros(inputVecs1.n_rows, inputVecs2.n_rows);
	// Loop though each vectors of inputVecs2
	for (int i=0; i < (int)inputVecs2.n_rows; i++){
		arma::mat repeated_vec;
	  repeated_vec.ones(inputVecs1.n_rows, inputVecs1.n_cols);
		// Assign column j with column j of vector [i] of second set
		for (int j=0; j < (int)inputVecs1.n_cols; j++)
			repeated_vec.col(j).fill(inputVecs2(i, j));
		repeated_vec = inputVecs1 - repeated_vec;
		repeated_vec %= repeated_vec;
		arma::vec distVec = arma::sum(repeated_vec, 1);
		results.col(i) = distVec;
	}
	return results;
}


arma::mat DatCustom::ConvertUtil::flattenImageToArmaMat(cv::Mat img, bool debug){
	// 1. Takes all colors in the image
	int flattenSize = img.rows*img.cols;
	if (debug) 	std::cout << "Flatten size: " << flattenSize << std::endl;
	int numChannels;
	std::vector<cv::Mat> planes = DatCustom::ImUtil::getChannels(img, &numChannels);
	// Creates matrix numChannels x flattenSize
	arma::mat vec_mats;
	vec_mats.zeros(numChannels, flattenSize);
	if (debug) std::cout << "Converting Image to arma mat" << std::endl;
	for (int i = 0; i < numChannels; i++){
		arma::mat arma_mat = DatCustom::ConvertUtil::fromCVMat(planes[i]);
		if (debug) std::cout << "Size: " << arma_mat.size() << std::endl;
		arma_mat.reshape(1, flattenSize);
		vec_mats.row(i) = arma_mat.row(0);
	}
	return vec_mats;
}

arma::mat getInitCentersArmadillo(arma::mat vec_mats, int numClusters,
	const arma::gmm_seed_mode spreadMode=arma::static_subset,	bool debug=false){
	arma::mat centers;
	if (debug){
		std::cout << "Running Kmeans" << std::endl;
	}
	bool status = arma::kmeans(centers, vec_mats,
		 	(arma::uword)numClusters, spreadMode, 0, false);

	if(status == false)
	{
		std::cout << "Initial Mean calculation failed" << std::endl;
	}
	return centers;
}


arma::mat getKCenters(arma::mat vec_mats, int numClusters, bool debug){
	arma::mat centers(vec_mats.n_rows, numClusters, arma::fill::zeros);
	// Building distance matrix
	arma::mat distMat = DatCustom::LinAlgUtil::cdist(vec_mats, vec_mats);
	throw DatCustom::Exception::NotImplementedException();
	return centers;
}



int DatCustom::ProbUtil::weightedProbRandomPick(const arma::mat& probVec, double cutoff, bool debug){
	double min_prob = fmax(cutoff, probVec.min());
	arma::mat distribution_count =  arma::ceil(probVec/min_prob);
	if (debug) std::cout << "Probs: " << std::endl;
	if (debug) probVec.print(std::cout);
	int sum = 0;
	arma::mat distribution;
	distribution.ones(1, arma::accu(distribution_count));
	if (debug) std::cout << "distribution size: " << distribution.size() << std::endl;
	for (int j = 0; j < (int)distribution_count.n_cols; j++){
		if (debug) std::cout << "Cols: " << sum  << ", " << sum+distribution_count[j]-1 << std::endl;
		if (distribution_count[j] == 0) continue;
		distribution.cols(sum, sum+distribution_count[j]-1) *= j;
		sum += distribution_count[j];
	}
	arma::arma_rng::set_seed_random();
	int index_selected = distribution[(int)arma::floor(arma::randu(1)*distribution.n_cols)[0]];
	return index_selected;
}


int DatCustom::ProbUtil::weightedProbRandomPickCumSum(const arma::mat& probVec, bool debug){
	arma::vec cumSum = arma::cumsum(probVec);
	arma::arma_rng::set_seed_random();
	double prob = arma::randu(1)[0];
	int chosenIndex = 0;
	while(prob > cumSum[chosenIndex]) chosenIndex++;
	if (debug) std::cout << "Chosen prob: " << prob << "\nChosen Index: " << chosenIndex << std::endl;
	return chosenIndex;
}


arma::mat initializeKmeansPP(arma::mat vecsMat, int numClusters, bool debug){
	arma::arma_rng::set_seed_random();
	arma::mat centers;
	centers.zeros(vecsMat.n_rows, numClusters);
	// Select an element using uniform distribution
	arma::arma_rng::set_seed_random();
	centers.col(0) = vecsMat.col((int)arma::floor(arma::randu(1)*vecsMat.n_cols)[0]);
	for (int i = 1; i < numClusters; i++){
		// D^2 Mat
		arma::mat distMat = DatCustom::LinAlgUtil::cdist(vecsMat.t(), centers.cols(0, i-1).t());
		arma::mat sqDistVec = arma::min(distMat, 1);
		arma::mat sqDistVecProb = sqDistVec / arma::accu(sqDistVec);
		int index_selected = DatCustom::ProbUtil::weightedProbRandomPickCumSum(sqDistVecProb, debug);
		centers.col(i) = vecsMat.col(index_selected);

		if (debug) sqDistVecProb.print(std::cout);
	}
	return centers;
}


arma::mat initializeKmeansPPModified(arma::mat vecsMat, int numClusters, bool debug){
	arma::mat centers;
	centers.zeros(vecsMat.n_rows, numClusters);
	// Select an element using uniform distribution
	arma::arma_rng::set_seed_random();
	centers.col(0) = vecsMat.col((int)arma::floor(arma::randu(1)*vecsMat.n_cols)[0]);
	arma::mat distMat = DatCustom::LinAlgUtil::cdist(vecsMat.t(), centers.cols(0, 0).t());
	 // This is what Arthur/Vassilvitskii tried on scikit learn code, but did not report
   // specific results for other than mentioning in the conclusion
   // that it helped.
	int nLocalTrials = 2 + (int) log(numClusters);
	for (int i = 1; i < numClusters; i++){
		// D^2 Mat
		arma::mat sqDistVec = arma::min(distMat, 1);
		arma::mat sqDistVecProb = sqDistVec / arma::accu(sqDistVec);
		int index_selected = DatCustom::ProbUtil::weightedProbRandomPickCumSum(sqDistVecProb, debug);

		arma::mat tryingCenters = centers;
		tryingCenters.col(i) = vecsMat.col(index_selected);
		arma::mat currentBestDistMat = DatCustom::LinAlgUtil::cdist(vecsMat.t(), tryingCenters.cols(0, i).t());
		double currentBestDist = arma::accu(arma::min(currentBestDistMat, 1));

		for (int j = 1; j < nLocalTrials; j++){
			int index = DatCustom::ProbUtil::weightedProbRandomPickCumSum(sqDistVecProb, debug);
			tryingCenters.col(i) = vecsMat.col(index);
			arma::mat candidateDistMat = DatCustom::LinAlgUtil::cdist(vecsMat.t(), tryingCenters.cols(0, i).t());
			double newDist = arma::accu(arma::min(candidateDistMat, 1));
			if (newDist < currentBestDist){
				index_selected = index;
				currentBestDist = newDist;
				currentBestDistMat = candidateDistMat;
			}
		}
		centers.col(i) = vecsMat.col(index_selected);
		distMat = currentBestDistMat;
		if (debug) sqDistVecProb.print(std::cout);
	}
	return centers;
}


enum INTERTIA{MAX_SPREAD_CENTROIDS, MIN_DATA_CENTROIDS};
arma::mat initializeKmeansPPwithInertia(arma::mat vecsMat, int numClusters, int numIters=3, INTERTIA inertia=MIN_DATA_CENTROIDS, bool debug=false){
	arma::mat centers = initializeKmeansPPModified(vecsMat, numClusters, debug);
	arma::mat distMat;
	double currentInertia;
	if (inertia==MAX_SPREAD_CENTROIDS){
		distMat = DatCustom::LinAlgUtil::cdist(centers.t(), centers.t());
		arma::mat sqDistVec = arma::min(distMat, 1);
		currentInertia = -arma::accu(sqDistVec);
	}
	else if (inertia == MIN_DATA_CENTROIDS){
		distMat = DatCustom::LinAlgUtil::cdist(vecsMat.t(), centers.t());
		arma::mat sqDistVec = arma::min(distMat, 1);
		currentInertia = arma::accu(sqDistVec);
	}
	double inertia_msr;

	if (debug) std::cout << "Inital inertia: " << currentInertia << std::endl;;
	for (int i = 1; i < numIters; i++){
		arma::mat newCenters = initializeKmeansPPModified(vecsMat, numClusters, debug);
		if (inertia==MAX_SPREAD_CENTROIDS){
			distMat = DatCustom::LinAlgUtil::cdist(newCenters.t(), newCenters.t());
			arma::mat sqDistVec = arma::min(distMat, 1);
			inertia_msr = -arma::accu(sqDistVec);
		}
		else if (inertia == MIN_DATA_CENTROIDS){
			distMat = DatCustom::LinAlgUtil::cdist(vecsMat.t(), newCenters.t());
			arma::mat sqDistVec = arma::min(distMat, 1);
			inertia_msr = arma::accu(sqDistVec);
		}

		if (debug) std::cout << "Inertia_" << i+1 << ": " << fabs(inertia_msr) << std::endl;
		if (inertia_msr < currentInertia){
			centers = newCenters;
			currentInertia = inertia_msr;
		}
	}
	return centers;
}


cv::Mat DatCustom::ClusterUtil::imageKmeans(cv::Mat img, int numClusters, arma::mat* pLabelMat, bool debug){
	// 1. Takes all colors in the image
	int flattenSize = img.rows*img.cols;
	arma::mat vec_mats = ConvertUtil::flattenImageToArmaMat(img, debug);
	int numChannels = vec_mats.n_rows;

	if (debug){
		std::cout << "Image Vecmat size: (" << vec_mats.n_rows << "," << vec_mats.n_cols << ")" << std::endl;
		std::cout << "Checking Vecmat containing NaN: " << vec_mats.has_nan() << std::endl;
		std::cout << "Checking Vecmat containing Inf: " << vec_mats.has_inf() << std::endl;
	}
	// 2. Use arma kmeans function
	arma::mat centers = initializeKmeansPPwithInertia(vec_mats, numClusters, 3, MIN_DATA_CENTROIDS, debug);
	int numIters = 10;
	bool printDuringRunning = debug;
	if (debug){
		std::cout << "Running Kmeans" << std::endl;
	}
	bool status = arma::kmeans(centers, vec_mats, (arma::uword)numClusters, arma::keep_existing,
		 	numIters, printDuringRunning);

	if(status == false)
	{
		std::cout << "clustering failed" << std::endl;
	}

	if (debug) centers.print("means:");
	// centers: each a column vector
	// 3. reassign clusters
	if (debug) std::cout << "Begining Distance calculation" << std::endl;
	arma::mat distmat = LinAlgUtil::cdist(vec_mats.t(), centers.t());
	arma::ucolvec indicesMat = arma::index_min(distmat, 1);
	if (pLabelMat != NULL){
		pLabelMat->zeros(flattenSize, 1);
		pLabelMat->col(0) = arma::conv_to<arma::vec>::from(indicesMat);
		pLabelMat->reshape(img.rows, img.cols);
	}
	std::vector<cv::Mat> planes(3);
	for (int i = 0; i < numChannels; i++){
		arma::mat arma_mat;
		arma_mat.zeros(1, flattenSize);
		for (int j = 0; j < flattenSize; j++)
			arma_mat(0, j) = centers(i, indicesMat[j]);
		arma_mat.reshape(img.rows, img.cols);
		planes[i] = ConvertUtil::fromArmaMat(arma_mat);
	}
	// 4. return kmeaned image
	cv::Mat clusteredImage;
	cv::merge(planes.data(), numChannels, cv::_OutputArray(clusteredImage));
	return clusteredImage;
}


cv::Mat DatCustom::ClusterUtil::imageMiniBatchKmeans(cv::Mat img, int numClusters,
	 	int batchSize, arma::mat* pLabelMat, bool debug){
	// 1. Takes all colors in the image
	int flattenSize = img.rows*img.cols;
	arma::mat vec_mats = ConvertUtil::flattenImageToArmaMat(img, debug);
	int numChannels = vec_mats.n_rows;
	// 2. Use minibatch kmeans
	arma::mat centers = initializeKmeansPPwithInertia(vec_mats, numClusters, 5, MIN_DATA_CENTROIDS, debug);
	if (debug){
		std::cout << "Centers: " << std::endl;
		centers.print(std::cout);
	}

	batchSize = fmin(batchSize, vec_mats.n_cols);
	arma::vec countVec;
	countVec.zeros(numClusters);
	arma::mat distmat;
	arma::ucolvec indicesMat;
	int numIters = 300;
	for (int i=0; i < numIters; i++){
		if (debug) std::cout << "Current Iterations: " << i+1 << std::endl;
		arma::mat M(vec_mats.n_rows, batchSize, arma::fill::zeros);
		for (int j = 0; j < batchSize; j++){
			arma::arma_rng::set_seed_random();
			M.col(j) = vec_mats.col((int)arma::floor(arma::randu(1)*vec_mats.n_cols)[0]);
		}
		if (debug){
			std::cout << "Running cdist..." << std::endl;
		}
		distmat = LinAlgUtil::cdist(M.t(), centers.t());
		if (debug) std::cout << "Finished running Cdist, running index min..." << std::endl;
		indicesMat = arma::index_min(distmat, 1);
		if (debug) std::cout << "Finished running index min...Updating clusters..." << std::endl;
				// split matrix into 2 parts
		std::vector<int> not_reassign;
		std::vector<int> reassign;
		for (int j = 0; j < numClusters; j++){
			// If num elemnt < some threshold, reassign
			if (countVec[j] < arma::max(countVec)*0.1){
				reassign.push_back(j);
			}
			else not_reassign.push_back(j);
		}
		if (reassign.size() > 0){
			// reassign
			arma::arma_rng::set_seed_random();
			arma::vec new_samples = arma::floor(arma::randu(reassign.size())*vec_mats.n_cols);
			int j = 0;
			arma::vec notVec(not_reassign.size(), arma::fill::zeros);
			for (auto it = not_reassign.begin(); it != not_reassign.end(); it++){
				notVec[j] = countVec[*it];
				j++;
			}
			if (debug) {
				notVec.print("Not assigned vec: ");
				std::cout << std::endl;
			}
			j = 0;
			double min_notVec = arma::min(notVec);
			for (auto it = reassign.begin(); it != reassign.end(); it++){
				centers.col(*it) = vec_mats.col(new_samples[j]);
				j++;
				// reset counts of reassigned centers, but don't reset them too small
				// to avoid instant reassignment. This is a pretty dirty hack as it
				// also modifies the learning rates.
				countVec[*it] = min_notVec;
			}
			
		}
		for (int centerIdx = 0; centerIdx < numClusters; centerIdx++){
			arma::umat centerMask = indicesMat == centerIdx;
			int count = arma::accu(centerMask);
			centers.col(centerIdx) *= countVec[centerIdx];
			for (int j = 0; j < (int)indicesMat.n_rows; j++) {
				if ((int)indicesMat[j] == (int)centerIdx) centers.col(centerIdx) +=  vec_mats.col(j);
			}
			countVec[centerIdx] += count;
			centers.col(centerIdx) /= (double)countVec[centerIdx];
		}

		if (debug){
			countVec.print("Count Vec: ");
			std::cout << std::endl;
			std::cout << "Index max : " << arma::index_max(countVec) << std::endl;
		}
	}
	// 3. reassign clusters
	if (debug) std::cout << "Begining Distance calculation" << std::endl;

	if (debug) std::cout << "Finished Distance calculation, placing label matrix" << std::endl;
	distmat = LinAlgUtil::cdist(vec_mats.t(), centers.t());
	indicesMat = arma::index_min(distmat, 1);
	if (pLabelMat != NULL){
		pLabelMat->zeros(flattenSize, 1);
		pLabelMat->col(0) = arma::conv_to<arma::vec>::from(indicesMat);
		pLabelMat->reshape(img.rows, img.cols);
	}
	if (debug) std::cout << "Converting back to opencv matrix" << std::endl;
	std::vector<cv::Mat> planes(3);
	if (debug) std::cout << "Number of channels: " << numChannels << std::endl;
	for (int i = 0; i < numChannels; i++){
		arma::mat arma_mat;
		arma_mat.zeros(1, flattenSize);
		for (int j = 0; j < flattenSize; j++){
			arma_mat(0, j) = centers(i, indicesMat[j]);
		}
		arma_mat.reshape(img.rows, img.cols);
		planes[i] = ConvertUtil::fromArmaMat(arma_mat);
	}
	// 4. return kmeaned image
	cv::Mat clusteredImage;
	cv::merge(planes.data(), numChannels, cv::_OutputArray(clusteredImage));
	return clusteredImage;
}
