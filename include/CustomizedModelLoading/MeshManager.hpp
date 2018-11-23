#ifndef __CUSTOMIZED_MODEL_LOADING_MESH_MANAGER_HPP__
#define __CUSTOMIZED_MODEL_LOADING_MESH_MANAGER_HPP__
#include <map>
#include "Mesh.h"


namespace DatCustom{
	namespace Graphics{
		using MeshPtr = std::shared_ptr<DatCustom::Graphics::Mesh>;
		class MeshManager{
				MeshManager();
				std::map<std::string, MeshPtr> loadedMeshFiles;
				MeshPtr createMeshFromCTM(CTM::Mesh& ctmMesh);
			private:
				MeshManager(const MeshManager& rhs);
				MeshManager& operator=(const MeshManager& rhs);
			public:
				MeshPtr loadStaticMesh(const char* filePath);
				static MeshManager& instance();
		};
	}
}
#endif
