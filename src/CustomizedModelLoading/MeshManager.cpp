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
	if (!ctmMesh.HasTexCoords()) {
		// Get vector of vertex, color, etc
		return std::unique_ptr<glMesh>(new ColoredMesh(ctmMesh));
	}
	else{
		try{
			return std::unique_ptr<glMesh>(new TexturedMesh(ctmMesh));
		} catch (std::logic_error e){
			printf("Warning: Failed to load texture, using default color\n");
				return std::unique_ptr<glMesh>(new ColoredMesh(ctmMesh));
		}
	}
}


DatCustom::Graphics::MeshPtr MeshManager::loadStaticMesh(const char* filePath){
	CTM::Mesh ctmMesh;
	using std::string;
	std::string fileExt = UpperCase(ExtractFileExt(std::string(filePath)));
	try{
		CTM::ImportMesh(filePath, &ctmMesh);
		std::string sfilePath (filePath);
		std::string delimiter = ".";
		std::string filename = sfilePath.substr(0, sfilePath.find(delimiter)); // token is "scott"
		if (ctmMesh.mTexFileName == ""){
			ctmMesh.mTexFileName = std::string(filename) + "_texture.png";
		}
		MeshPtr newMesh = createMeshFromCTM(ctmMesh);
		loadedMeshFiles.insert({std::string(filePath), newMesh});
		return newMesh;
	}
	catch (std::runtime_error e){
		// Load the mesh normally
		return MeshPtr(new TexturedModel(filePath));
	}
}
