#pragma once

#include <boost/noncopyable.hpp>
#include <list>
#include <vector>

#include <SDL/SDL.h>

#include "../Log.h"
#include "../Cpp11.h"
#include "InputSystemBase.h"
#include "InputContainer.h"


// be able to map and remap device input at any stage of the Input init !
class SDLInput: private boost::noncopyable {
public:
	static constexpr int GamepadMax = 32768;

	static constexpr int GamepadXBoxA = 0;
	static constexpr int GamepadXBoxB = 1;
	static constexpr int GamepadXBoxC = 2;
	static constexpr int GamepadXBoxD = 3;
	static constexpr int GamepadXBoxStart = 7;

	SDLInput();
	virtual ~SDLInput();
	void updateContainer(InputSystemBase::ContainerArray &, float);

	size_t maxInputDevices() const {
		// one keyboard and 4 gamepads ...
		return 5;
	}

	DeviceList availableInputDevices() const;
	//bool isGamepadAvailable();

private:

	SDL_Event m_event;
	//std::vector<SDL_Joystick *> m_gamepadHandles;
	std::map<int, SDL_Joystick *> m_gamepadHandles;
};
