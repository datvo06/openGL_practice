#include <DatCustomUtils/FileUtils/FileUtils.hpp>
#include <CustomizedModelLoading/MeshManager.hpp>
#include <CustomizedModelLoading/Model.h>
#include <CTM/meshio.h>
#include <CTM/common.h>


using MeshManager = DatCustom::Graphics::MeshManager;

MeshManager::MeshManager(){}

MeshManager& MeshManager::instance(){
	static MeshManager instance = MeshManager();
	return instance;
}

DatCustom::Graphics::MeshPtr MeshManager::createMeshFromCTM(CTM::Mesh& ctmMesh){
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


DatCustom::Graphics::MeshPtr MeshManager::loadStaticMesh(const char* filePath){
	CTM::Mesh ctmMesh;
	using std::string;
	std::string fileExt = UpperCase(ExtractFileExt(std::string(filePath)));
	try{
		CTM::ImportMesh(filePath, &ctmMesh);
		MeshPtr newMesh = createMeshFromCTM(ctmMesh);
		loadedMeshFiles.insert({std::string(filePath), newMesh});
		return newMesh;
	}
	catch (std::runtime_error e){
		// Load the mesh normally
		return MeshPtr(new TexturedModel(filePath));
	}
}
