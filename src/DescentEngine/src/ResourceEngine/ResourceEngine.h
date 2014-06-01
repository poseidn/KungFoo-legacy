#pragma once

#include <utility>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <unordered_map>

#include "../Cpp11.h"
#include "../Log.h"
#include "../OpenGLInclude.h"

#include "../TextureRef.h"
#include "../SoundEngine/SoundPtr.h"

class ResourceManager {
public:
	virtual void freeResources()= 0;
};

class ResourceEngineAbstract: public ResourceManager, private boost::noncopyable {
public:

	std::string defaultSoundExt() const {
		return ".ogg";
	}

	std::string getImagePrefix() const {
		return "images/";
	}

	std::string getSoundPrefix() const {
		return "sounds/";
	}

	std::string getLevelPrefix() const {
		return "levels/";
	}

	std::string getScriptPrefix() const {
		return "scripts/";
	}

	virtual ~ResourceEngineAbstract();

	void freeResources() CPP11_OVERRIDE;

	// todo: don't load the same image twice, but return a ref to the actually loaded
	virtual TexturePtr loadImage(std::string const& imageName, unsigned int frames = 1) = 0;

	virtual SoundPtr loadSound(std::string const& soundName) = 0;
	virtual MusicPtr loadMusic(std::string const& musicName) = 0;

	virtual std::string loadLevel(std::string const& levelName) = 0;

	virtual std::string loadScript(std::string const& scriptName) = 0;

	virtual std::string getScriptPath(std::string const & scriptName) {
		logging::NotImplemented();
		return "";
	}

	// just put an already loaded texture into the texture cache
	virtual void preloadImage(std::string const& imageName, GLuint glId, size_t frameCount = 1) = 0;

protected:

	// called by the constructor of this class to give child classes a chance
	// to free the image, for example delete it from the grafix ram
	virtual void freeTexture(TexturePtr tex) = 0;

	virtual void freeSound(SoundPtr snd) = 0;

	virtual void freeMusic(MusicPtr msc) = 0;
	/*{
	 std::cout << "abstract free texture";
	 }*/

	std::pair<bool, TexturePtr> checkTextureCache(std::string const& imageName) const {
		if (m_textures.find(imageName) != m_textures.end()) {
			// already loaded this texture
			//logging::Info() << "Returning cached texture " << imageName;
			return std::pair<bool, TexturePtr>(true, TexturePtr(m_textures.at(imageName)));
		} else {
			return std::pair<bool, TexturePtr>(false, TexturePtr());
		}
	}

//typedef boost::ptr_map<std::string, Texture> TexturesMap;
	typedef std::unordered_map<std::string, TexturePtr> TexturesMap;
	TexturesMap m_textures;

	typedef std::unordered_map<std::string, SoundPtr> SoundMap;
	SoundMap m_sounds;

	typedef std::unordered_map<std::string, MusicPtr> MusicMap;
	MusicMap m_music;
};

#ifdef USE_SDL

#include "ResourceEngineSDL.h"

#endif

#ifdef USE_ANDROID

#include "ResourceEngineAndroid.h"

#endif

