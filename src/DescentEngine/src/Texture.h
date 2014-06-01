#pragma once

#include <memory>
#include <string>
#include <boost/noncopyable.hpp>

#include "OpenGLInclude.h"

#include "Common_OpenGL/GLSupport.h"

#include "Log.h"

#define DESCENT_TEXTURE_NAME_DEBUG 1

class Texture: public boost::noncopyable {
public:
	Texture(unsigned int height, unsigned int width, GLint numberOfColors, GLenum textureFormat,
			void * pixelPointer, unsigned int animFrames = 1) :
			m_animationFrames(animFrames), m_height(height), m_width(width), m_numberOfColors(numberOfColors), m_textureFormat(
					textureFormat), m_pixelPointer(pixelPointer), m_bound(false) {

	}

	Texture(GLuint id, size_t frameCount = 1) :
			m_animationFrames(frameCount), m_height(0), m_width(0), m_numberOfColors(0), m_textureFormat(0), m_pixelPointer(
					nullptr), m_glId(id), m_bound(true) {

	}

	~Texture() {
	}

	GLuint getID() const {
		return m_glId;
	}

	bool isAnimated() const {
		return (m_animationFrames > 1);
	}

	size_t getAnimationFrames() const {
		return m_animationFrames;
	}

	// this has to be done here because we have to ensure, that
	// all OpenGl calls happen on the Render Thread
	void ensureOpenGLBind() {
		if (!m_bound) {

#ifdef DESCENT_TEXTURE_NAME_DEBUG
			logging::Info() << "Binding texture " << getTextureName() << " to OpenGL";
#endif

			GL_CHECK_ERROR(glGenTextures(1, &m_glId));

// Bind the texture object
			GL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, m_glId));

// Set the texture's stretching properties
			/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

			// this will not use any kind of filtering to blur / anti-alise the textures
			// setting when the texture is farther away than it's original size
			GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			// setting when the textre is closer than it's original size
			GL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

// Edit the texture object's image data using the information SDL_Surface gives us
			GL_CHECK_ERROR(
					glTexImage2D(GL_TEXTURE_2D, 0, m_numberOfColors, m_width, m_height, 0, m_textureFormat, GL_UNSIGNED_BYTE, m_pixelPointer));

			m_bound = true;
		}

	}

#ifdef DESCENT_TEXTURE_NAME_DEBUG
	void setTextureName(std::string const& name) {
		m_textureName = name;
	}

	std::string const& getTextureName() const {
		return m_textureName;
	}
#endif

#ifdef USE_SDL
	SDL_Surface* m_sdlSurface;
#endif

private:
#ifdef DESCENT_TEXTURE_NAME_DEBUG
	std::string m_textureName;
#endif

	unsigned int m_animationFrames;
	unsigned int m_height;
	unsigned int m_width;
	GLint m_numberOfColors;
	GLenum m_textureFormat;
	void * m_pixelPointer;

	GLuint m_glId;
	bool m_bound;

};

