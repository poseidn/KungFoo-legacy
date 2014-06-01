#pragma once

#include <boost/noncopyable.hpp>

#include "../OpenGLInclude.h"

#include "../Visuals/ScreenTransform.h"
#include "../VectorTypes.h"

#include "../InterfaceInitData.h"
#include "SDLOpenGL_State.h"

class SDLInterfaceInitData: public InterfaceInitData {
public:

	SDLInterfaceInitData() :
			Resolution(0, 0), ResolutionSet(false), Fullscreen(false) {
	}

	SDLInterfaceInitData(Vector2Uint reso, bool fscreen = false) :
			Resolution(reso), ResolutionSet(true), Fullscreen(fscreen) {
	}

	/*size_t ResolutionX;
	 size_t ResolutionY;*/
	Vector2Uint Resolution;

	bool ResolutionSet;

	bool Fullscreen;
};

class SDLOpenGLInterface {
public:

	typedef SDLInterfaceInitData InterfaceDataType;

	void init(SDLInterfaceInitData const& data, SDLOpenGL_State & state);

	ScreenTransform const& getScreenTransform() const {
		return m_transform;
	}

private:

	ScreenTransform m_transform;
};
