#ifndef __CTM_MESH_CUSTOMIZED_MESH_ADAPTOR_HPP__
#define __CTM_MESH_CUSTOMIZED_MESH_ADAPTOR_HPP__
#include "Mesh.h"

class MeshFactory {
	MeshFactory();
	public:
		static MeshFactory instance();
		MeshFactory operator=(const MeshFactory& rhs);
		std::unique_ptr<glMesh> createMesh(CTM::Mesh& ctmMesh);
};
#endif
