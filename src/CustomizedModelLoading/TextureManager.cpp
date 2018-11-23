#include <CustomizedModelLoading/TextureManager.hpp>

using DatCustom::Graphics::TextureManager;
TextureManager::TextureManager(){
}


TextureManager& TextureManager::instance(){
	static TextureManager textureManager;
	return textureManager;
}


DatCustom::Graphics::TexturePtr TextureManager::loadTextureFromFile(const char* filePath, std::string typeName){
	auto it = loadedTextures.find(filePath);
	if (it == loadedTextures.end()){
		GLint textureID = TextureFromFile(filePath, "");
		TexturePtr pTexture(new Texture());
		pTexture->id = textureID;
		pTexture->type = typeName;
		loadedTextures.insert({filePath, pTexture});
		// Only return a copy
		return TexturePtr(new Texture({
				loadedTextures[filePath]->id,
				loadedTextures[filePath]->type}));
	}
	else{
		// Only return a copy
		return TexturePtr(new Texture({it->second->id, typeName}));
	}
}
