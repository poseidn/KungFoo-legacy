#include "ResourceEngineAndroid.h"

#include "../Log.h"
#include "../Texture.h"

void ResourceEngineAndroid::preloadImage(std::string const& imageName, GLuint glId, size_t frameCount) {
	TexturePtr texPtr(new Texture(glId, frameCount));
	m_textures[imageName] = texPtr;
	logging::Info() << "image " << imageName << " preloaded" << std::endl;
}

TexturePtr ResourceEngineAndroid::loadImage(std::string const& imageName, unsigned int frames) {
	auto res = checkTextureCache(imageName);
	if (res.first) {
		return res.second;
	} else {
		int id = m_javaInterface->loadImage(imageName);

		Texture * texPtr = new Texture(id, frames);
		m_textures[imageName] = texPtr;
		return texPtr;
	}
}

std::string ResourceEngineAndroid::loadLevel(std::string const& levelName) {
	//logging::Fatal() << " not implemented";
	if (!assertInterface())
		return "";

	const std::string fileName(getLevelPrefix() + levelName + ".xml");
	std::string result = m_javaInterface->readTextFile(fileName);

	logging::Info() << "C++ level loaded : " << result;

	return result;
}

std::string ResourceEngineAndroid::loadScript(std::string const& scriptName)
{
	if (!assertInterface())
		return "";

	const std::string fileName(getScriptPrefix() + scriptName + ".lua");
	std::string result = m_javaInterface->readTextFile(fileName);

	logging::Info() << "lua script loaded : " << result;

	return result;
}
