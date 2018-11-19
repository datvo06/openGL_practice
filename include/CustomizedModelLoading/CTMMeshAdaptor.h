#ifndef __CTM_MESH_CUSTOMIZED_MESH_ADAPTOR_HPP__
#define __CTM_MESH_CUSTOMIZED_MESH_ADAPTOR_HPP__
#include "Mesh.h"
#include <CTM/mesh.h>

class MeshFactory {
	public:
		std::unique_ptr<glMesh> getMesh(CTM::Mesh ctmMesh);
}
#endif
