#ifndef __CUSTOMIZED_MODEL_LOADING_TEXTURE_MANAGER_HPP__
#define __CUSTOMIZED_MODEL_LOADING_TEXTURE_MANAGER_HPP__
#include <CustomizedModelLoading/Texture.h>
#include <map>
#include <memory>

namespace DatCustom{
	namespace Graphics{
		using TexturePtr = std::shared_ptr<DatCustom::Graphics::Texture>;
		class TextureManager{
			TextureManager();
			std::map<std::string, TexturePtr> loadedTextures;
			private:
			TextureManager(const TextureManager& rhs);
			TextureManager& operator=(const TextureManager& rhs);
			public:
			TexturePtr loadTextureFromFile(const char* filePath, std::string typeName="texture_diffuse");
			static TextureManager& instance();
		};
	}
}
#endif
