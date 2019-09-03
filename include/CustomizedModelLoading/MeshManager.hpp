#ifndef __CUSTOMIZED_MODEL_LOADING_MESH_MANAGER_HPP__
#define __CUSTOMIZED_MODEL_LOADING_MESH_MANAGER_HPP__
#include <map>
#include "Mesh.h"
#include "BlendShapeMesh.h"


namespace DatCustom{
	namespace Graphics{
		using MeshPtr = std::shared_ptr<DatCustom::Graphics::Mesh>;
		using BlendShapeMeshPtr = std::shared_ptr<DatCustom::Graphics::BlendShapeMesh>;
		class MeshManager{
				MeshManager();
				std::map<std::string, MeshPtr> loadedMeshFiles;
				MeshPtr createMeshFromCTM(CTM::Mesh& ctmMesh);
				BlendShapeMeshPtr createBlendShapeMeshFromListCTMs(std::vector<CTM::Mesh>& listCtmMeshes);
			private:
				MeshManager(const MeshManager& rhs);
				MeshManager& operator=(const MeshManager& rhs);
			public:
				MeshPtr loadStaticMesh(const char* filePath);
				BlendShapeMeshPtr loadStaticBlendShape(const char** filePaths, int numBlendShape);
				static MeshManager& instance();
		};
	}
}
#endif
