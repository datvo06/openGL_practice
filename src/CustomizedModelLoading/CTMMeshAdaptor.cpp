#include <CustomizedModelLoading/CTMMeshAdaptor.h>


MeshFactory::MeshFactory(){}

MeshFactory MeshFactory::instance(){
	static MeshFactory instance = MeshFactory();
	return instance;
}

std::unique_ptr<glMesh> MeshFactory::createMesh(CTM::Mesh& ctmMesh){
	if (!ctmMesh.HasNormals()){
		// Let's calculate normals for it
		ctmMesh.CalculateNormals();
	}
	// Still assigning colors, but doing nothing with it for now
	if (ctmMesh.HasColors()){
		// Get vector of vertex, color, etc
		return std::unique_ptr<glMesh>(new ColoredMesh(ctmMesh));
	}
	else {
		return std::unique_ptr<glMesh>(new TexturedMesh(ctmMesh));
	}
}
