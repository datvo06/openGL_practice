#include <FaceModel/PCAFaceModel.hpp>


std::unordered_map<std::string, Eigen::MatrixXf> DatCustom::FaceModel::getBFMParams(mat_t* pMatFile){
	matvar_t *pPCAModel = Mat_VarRead(pMatFile, "model");
	// Get the neccessary fields
	matvar_t **pFields = (matvar_t **)pPCAModel->data;
	int numFields = (int)pPCAModel->nbytes/pPCAModel->data_size;
	// Need to use Eigen's allocator
	std::unordered_map<std::string, Eigen::MatrixXf> pFieldsDict;
	for (int i = 0; i < numFields; i++){
		// If the rank is larger than 2, skip
		if (pFields[i]->rank > 2) continue;
		// Let's generate new Eigen matrix and copy data over
		// Let's use float for now
		Eigen::MatrixXf newMat;
		if (pFields[i]->rank == 1){
			newMat.resize(1, pFields[i]->dims[0]);
		}
		else {
			newMat.resize((int)pFields[i]->dims[0], (int)pFields[i]->dims[1]);
		}
		if (pFields[i]->data_type == MAT_T_SINGLE){
			const float *xData = static_cast<const float*>(pFields[i]->data) ;
			newMat = Eigen::Map<const Eigen::Matrix<typename Eigen::MatrixXf::Scalar,
						 Eigen::MatrixXf::RowsAtCompileTime, Eigen::MatrixXf::ColsAtCompileTime, Eigen::ColMajor>>(
								 xData, (int)pFields[i]->dims[0], (int)pFields[i]->dims[1]);
		}
		else if (pFields[i]->data_type == MAT_T_DOUBLE){
			const double* xData = static_cast<const double*>(pFields[i]->data);
			newMat = Eigen::Map<const Eigen::Matrix<typename Eigen::MatrixXd::Scalar,
						 Eigen::MatrixXd::RowsAtCompileTime, Eigen::MatrixXd::ColsAtCompileTime, Eigen::ColMajor>>(
								 xData, (int)pFields[i]->dims[0], (int)pFields[i]->dims[1]).cast<float>();
		}
		pFieldsDict.insert({std::string(pFields[i]->name),  newMat});
	}
	Mat_VarFree(pPCAModel);
	return pFieldsDict;
}


void DatCustom::FaceModel::PCAFaceModelManager::recalculateNormals(){
	normals.setZero();
	// loop through each triangle
	for (int i = 0; i < indices.cols(); i++){
		Eigen::Vector3f a = finalShape.block<3, 1>(3*indices(0, i), 0),
				b = finalShape.block<3, 1>(3*indices(1, i), 0),
				c = finalShape.block<3, 1>(3*indices(2, i), 0);
		Eigen::Vector3f tempNorm = (b-a).cross(c-a);
		normals.col(indices(0, i)) += Eigen::VectorXf(tempNorm);
		normals.col(indices(1, i)) += Eigen::VectorXf(tempNorm);
		normals.col(indices(2, i)) += Eigen::VectorXf(tempNorm);
	}
	for (int i = 0; i < normals.cols(); i++){
		normals.col(i).normalize();
	}
	// normals = normals.array().rowwise()/counts.array().transpose();
}


void DatCustom::FaceModel::PCAFaceModelManager::updateModel(const Eigen::VectorXf& shapeParams,
		const Eigen::VectorXf& expressionParams,
	 	const Eigen::VectorXf& textureParams){

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	if (shapeParams.size() != 0 || expressionParams.size() != 0) {
		finalShape = shapeMean;
		//using eigen broadcasting here...
		if (shapeParams.size() != 0) {
			Eigen::ArrayXf weightedShapeParams = shapeEV.array()*shapeParams.array();
			lastIdentity = (shapePC.array().rowwise()*weightedShapeParams.transpose()).rowwise().sum();
		}
		finalShape += lastIdentity;

		if (expressionParams.size() != 0) {
			Eigen::ArrayXf weightedExpressionParams = expEV.array()*expressionParams.array();
			lastExpr = (expPC.array().rowwise() * weightedExpressionParams.transpose()).rowwise().sum();
			lastExpr += expMean;
			// lastExpr = (expPC.array().rowwise() * expressionParams.array().transpose()).rowwise().sum();
		}
		finalShape += lastExpr;
		glBufferSubData(GL_ARRAY_BUFFER, 0, this->shapeMean.size()* sizeof(float), &finalShape(0));
		recalculateNormals();
		glBufferSubData(GL_ARRAY_BUFFER,
				this->shapeMean.size()* sizeof(float),
			 	this->normals.size()* sizeof(float), &normals(0));
	}
	if (textureParams.size() != 0){
		Eigen::ArrayXf weightedColorParams = texEV.array()*textureParams.array();
		lastColor = (texPC.array().rowwise()*weightedColorParams.transpose()).rowwise().sum();
		finalColor = (texMean + lastColor)/127.0;
		glBufferSubData(GL_ARRAY_BUFFER, this->shapeMean.size()* sizeof(float)*2, this->texMean.size()*sizeof(float),
			&finalColor(0));
	}
}

void DatCustom::FaceModel::PCAFaceModelManager::setupMesh(){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	std::cout << "Buffer name: " << VBO << std::endl;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		 	this->shapeMean.size()*2* sizeof(float) + this->texMean.size()*sizeof(float),
		 	NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0,
		 	this->shapeMean.size()* sizeof(float),
		 	&finalShape(0));
	glBufferSubData(GL_ARRAY_BUFFER,
			this->shapeMean.size()* sizeof(float),
			this->normals.size()* sizeof(float),
			&normals(0));
	glBufferSubData(GL_ARRAY_BUFFER, this->shapeMean.size()* sizeof(float)*2, this->texMean.size()*sizeof(float),
			&finalColor(0));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 	indices.size() * sizeof(unsigned int),
			&indices(0), GL_STATIC_DRAW);



	glEnableVertexAttribArray(0);
	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(this->shapeMean.size()*sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(this->shapeMean.size()*sizeof(float)*2));
	glBindVertexArray(0);
}


void DatCustom::FaceModel::PCAFaceModelManager::updateModelParams(
		const std::unordered_map<std::string, Eigen::MatrixXf>& fieldDatas){
	if (fieldDatas.size() == 0)
		return;
	// indices for EBO, will be constant throught out the program
	this->indices = fieldDatas.at("tri").cast<unsigned int>();
	this->indices -= Eigen::Matrix<unsigned int, Eigen::Dynamic, Eigen::Dynamic>::Ones(this->indices.rows(), this->indices.cols());
	// Params to generate a morphable model
	this->shapeMean = fieldDatas.at("shapeMU");
	this->shapePC = fieldDatas.at("shapePC");
	this->shapeEV = fieldDatas.at("shapeEV");

	this->expMean = fieldDatas.at("expMU");
	this->expPC = fieldDatas.at("expPC");
	this->expEV = fieldDatas.at("expEV");
	
	this->texMean = fieldDatas.at("texMU");
	this->texPC = fieldDatas.at("texPC");
	this->texEV = fieldDatas.at("texEV");

	// Final for render, setup for initial mesh setup
	this->finalShape = this->shapeMean;
	this->finalColor = this->texMean/127.0;
	this->normals = Eigen::MatrixXf::Zero(3, (int)shapePC.size()/3);
	this->lastExpr = Eigen::MatrixXf::Zero(shapeMean.rows(), shapeMean.cols()); 
	this->lastIdentity = Eigen::MatrixXf::Zero(expMean.rows(), expMean.cols()); 
	this->lastColor = Eigen::MatrixXf::Zero(texMean.rows(), texMean.cols()); 

	this->nShapeParams = this->shapePC.cols();
	this->nExpressionParams = this->expPC.cols();
	this->nTextureParams = this->texPC.cols();
	recalculateNormals();
	setupMesh();
}


DatCustom::FaceModel::PCAFaceModelManager::PCAFaceModelManager(const std::unordered_map<std::string, Eigen::MatrixXf>& fieldDatas){
	updateModelParams(fieldDatas);
}


DatCustom::FaceModel::PCAFaceModelManager& DatCustom::FaceModel::PCAFaceModelManager::instance(){
	static mat_t* pMatFile = Mat_Open("model/BFM.mat", MAT_ACC_RDONLY);
	static std::unordered_map<std::string, Eigen::MatrixXf> defaultMap = DatCustom::FaceModel::getBFMParams(pMatFile);
	static PCAFaceModelManager defaultModel(defaultMap);
	static DatCustom::FaceModel::PCAFaceModelManager& instance = defaultModel;
	return instance;
}


void DatCustom::FaceModel::PCAFaceModelManager::Draw(Shader shader){
			shader.setBool("hasTexture", false);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			// draw mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glCullFace(GL_BACK);
	}

