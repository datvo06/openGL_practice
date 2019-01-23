#ifndef __DAT_CUSTOM__CUSTOMIZED_PCA_MODEL_MANAGER__
#define __DAT_CUSTOM__CUSTOMIZED_PCA_MODEL_MANAGER__
#include <unordered_map>
#include <string>
#include <Eigen/Eigen>
#include <Shader.h>
#include <matio.h>

namespace DatCustom{
	namespace FaceModel{
		std::unordered_map<std::string, Eigen::MatrixXf> getBFMParams(mat_t* pMatFile);
		class PCAFaceModelManager{
			private:
				PCAFaceModelManager(const std::unordered_map<std::string, Eigen::MatrixXf>& fieldDatas = {});
				PCAFaceModelManager(const PCAFaceModelManager& rhs);
				PCAFaceModelManager& operator=(const PCAFaceModelManager& rhs);
				GLuint VAO, VBO, EBO;
				void setupMesh();
				void recalculateNormals();
				Eigen::MatrixXf normals;

				Eigen::MatrixXf shapeMean;
				Eigen::MatrixXf shapePC;
				Eigen::MatrixXf shapeEV;

				Eigen::MatrixXf expMean;
				Eigen::MatrixXf expPC;
				Eigen::MatrixXf expEV;

				Eigen::MatrixXf texMean;
				Eigen::MatrixXf texPC;
				Eigen::MatrixXf texEV;
				Eigen::Matrix<unsigned int, Eigen::Dynamic, Eigen::Dynamic>  indices;
				Eigen::MatrixXf finalShape;
				Eigen::MatrixXf finalColor;

				Eigen::MatrixXf lastExpr;
				Eigen::MatrixXf lastIdentity;
				Eigen::MatrixXf lastColor;

				int nShapeParams;
				int nExpressionParams;
				int nTextureParams;
				void updateModelParams(const std::unordered_map<std::string, Eigen::MatrixXf>& fieldDatas = {});
			public:
				int getNExpParams(){
					return nExpressionParams;
				}
				int getNTextureParams(){
					return nTextureParams;
				}
				int getNShapeParams(){
					return nTextureParams;
				}
				/** instance function
				 * @brief update instances if 
				 */
				static PCAFaceModelManager& instance();
				/** 
				 * function UpdateModel
				 * @brief update the next model to be drawn/save, if the param is empty vector, the part will not be updated
				 * and will assume last state
				 * @param shapeParams (1, num_PCA_shape_param) - matrix
				 * @param expressionParams (1, num_PCA_shape_param) - matrix
				 * @param textureParams (1, num_PCA_texture_param) - matrix
				 */
				virtual void updateModel(const Eigen::VectorXf& shapeParams,
						const Eigen::VectorXf& expessionParams,
					 	const Eigen::VectorXf& textureParams);
				virtual void Draw(Shader shader);
				virtual GLuint getVAO() { return VAO; };
				virtual GLuint getVBO() { return VBO; };
				virtual GLuint getEBO() { return EBO; };
		};
	}
}

#endif
