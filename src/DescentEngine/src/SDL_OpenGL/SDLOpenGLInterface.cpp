#include "SDLOpenGLInterface.h"

#include <cmath>
#include <array>

#include "../Log.h"
#include "../Common_OpenGL/GLSupport.h"

void SDLOpenGLInterface::init(SDLInterfaceInitData const& data, SDLOpenGL_State & state) {
	logging::Info() << "Initializing SDL based OpenGL";

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		logging::Fatal() << "Video initialization failed: " << SDL_GetError();
	}

	int bpp = 0;
	int flags = 0;
	SDL_VideoInfo const* info = SDL_GetVideoInfo();

	if (!info) {
		logging::Fatal() << "Video query failed: " << SDL_GetError();
	}

	//const float screenRatio = 4.0 / 3.0;

	// go for 720p, ouya like
	/*	const float screenRatio = 16.0 / 9.0;
	 const int height = 720;
	 const float pixelPerTile = 50.0f;*/

	int height = 0;
	int width = 0;

	if (data.ResolutionSet) {
		height = data.Resolution.y();
		width = data.Resolution.x();

	} else {
		// take the curren native resolution
		height = info->current_h;
		width = info->current_w;

		/*		const float screenRatio = 16.0 / 9.0;
		 // use this for fullscreen
		 //const int height = 768;
		 height = 600;
		 width = std::floor(height * screenRatio);*/
	}

	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_WM_SetCaption("Fast Descent", "Fast Descent");

	// todo : in fullscreen mode it is flickering pretty strong ...
	// maybe we need some kind of hardware buffer there ?
	flags = SDL_OPENGL;	// | SDL_FULLSCREEN;

	if (data.Fullscreen) {
		flags |= SDL_FULLSCREEN;
		// hide mouse cursor
		// will automatically appear again once either <TAB> is used
		// or the game is terminated
		SDL_ShowCursor(0);
	}

	logging::Info() << "Setting video mode <" << width << ":" << height << ">, Fullscreen = "
			<< data.Fullscreen;

	if (SDL_SetVideoMode(width, height, bpp, flags) == 0) {

		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		exit(1);
	}

	// enable various OpenGl stuff
	GL_CHECK_ERROR(glEnable (GL_TEXTURE_2D));
	// allows to use transparent textures
	GL_CHECK_ERROR(glEnable (GL_BLEND));
	GL_CHECK_ERROR(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//GL_CHECK_ERROR(glClearColor(0.6f, 0.6f, 0.6f, 0.6f));
	GL_CHECK_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GL_CHECK_ERROR(glViewport(0, 0, width, height));

	GL_CHECK_ERROR(glClear (GL_COLOR_BUFFER_BIT));

	//GLSupport::setupParallelProjection(width, height, -1.0f, 1.0f);

	GL_CHECK_ERROR(glMatrixMode (GL_MODELVIEW));
	GL_CHECK_ERROR(glLoadIdentity());
	GL_CHECK_ERROR(glEnable (GL_DEPTH_TEST));
	GL_CHECK_ERROR(glDepthFunc (GL_LEQUAL));

	//setup lighting model
	//GL_CHECK_ERROR(glEnable(GL_LIGHTING));

	// does GL_SMOOTH need normals for each vertex ?
	//GL_CHECK_ERROR(glShadeModel(GL_SMOOTH));

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	// w component = 0 to have purely directional light
	GLfloat light_position[] = { 0.0, -15.0, -1.0, 1.0 };
	/*
	 GL_CHECK_ERROR(glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular));
	 GL_CHECK_ERROR(glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse));
	 GL_CHECK_ERROR(glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess));*/
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//GL_CHECK_ERROR(glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0));
	/*GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	 */

	/*m_transform = ScreenTransform(Vector2(pixelPerTile, pixelPerTile), Vector2(width, height));
	 // to have the walls - more or less - centered in the middle of the screen
	 m_transform.setOffset(Vector2(float(width) * 0.155f, 0.0f));*/

	m_transform = GLSupport::computeScreenTransform(width, height);
}

