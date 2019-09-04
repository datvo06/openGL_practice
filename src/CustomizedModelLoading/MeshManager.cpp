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
		}
		catch (std::logic_error e){
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
	catch (std::logic_error e){
		return MeshPtr(new TexturedModel(filePath));
	}
}


DatCustom::Graphics::BlendShapeMeshPtr MeshManager::createBlendShapeMeshFromListCTMs(std::vector<CTM::Mesh>& listCtmMeshes){
	if (!listCtmMeshes[0].HasNormals()){
		printf("Calculating normals for all the meshes...\n");
		// Let's calculate normals for it
		for (size_t i = 0; i < listCtmMeshes.size(); i++){
				listCtmMeshes[i].CalculateNormals();
		}
	}
	// Still assigning colors, but doing nothing with it for now
	if (!listCtmMeshes[0].HasTexCoords()) {
		printf("Pushing all meshes into one colored blendshape...\n");
		// Get vector of vertex, color, etc
		return std::unique_ptr<DatCustom::Graphics::ColoredBlendShapeMesh>(new DatCustom::Graphics::ColoredBlendShapeMesh(listCtmMeshes, true));
	}
	else{
		try{
			return std::unique_ptr<DatCustom::Graphics::TexturedBlendShapeMesh>(
					new TexturedBlendShapeMesh(listCtmMeshes));
		} catch (std::logic_error e){
			return std::unique_ptr<DatCustom::Graphics::ColoredBlendShapeMesh>(new DatCustom::Graphics::ColoredBlendShapeMesh(listCtmMeshes));
		}
	}
}

DatCustom::Graphics::BlendShapeMeshPtr MeshManager::loadStaticBlendShape(std::vector<std::string> filePaths){
	std::vector<CTM::Mesh> ctmMeshes;
	int numBlendShape = filePaths.size();
	for (int i = 0; i < numBlendShape; i++){
		CTM::Mesh ctmMesh;
		using std::string;
		std::string fileExt = UpperCase(ExtractFileExt(std::string(filePaths[i])));
		try{
			printf("Importing %dth mesh\n", (int)i);
			CTM::ImportMesh(filePaths[i].c_str(), &ctmMesh);
			std::string sfilePath (filePaths[i].c_str());
			std::string delimiter = ".";
			std::string filename = sfilePath.substr(0, sfilePath.find(delimiter));
			if (ctmMesh.mTexFileName == ""){
				ctmMesh.mTexFileName = std::string(filename) + "_texture.png";
			}
			ctmMeshes.push_back(ctmMesh);
		}
		catch (std::runtime_error e){
			// Failure in case file extension is not included
			printf("ERROR loading Blendshape: Filetype not supported!!\n");
		}
	}
	printf("Loaded %d meshes for blendshape\n", (int)filePaths.size());

	return createBlendShapeMeshFromListCTMs(ctmMeshes);
}
