#include "XboxInput.h"
#include "Utility.h"

namespace jl
{
	namespace XboxInput
	{
		bool isAxisDown(
			unsigned int controllerIndex,
			XboxInput::Axis axis,
			float triggerValue,
			float valueSpread)
		{
			return jl::Math::valueInRange<float, float, float>(
				sf::Joystick::getAxisPosition(
					controllerIndex,
					translateAxis(axis)), triggerValue, valueSpread);
		}

		bool isButtonDown(unsigned int controllerIndex, XboxInput::Buttons button)
		{
			return sf::Joystick::isButtonPressed(
				controllerIndex,
				translateButton(button));
		}


	// Check if compiled on unix
	#ifdef __unix || __unix__

		bool usingWindows()
		{
			return false;
		}
		bool usingUnix()
		{
			return true;
		}

		unsigned int translateButton(XboxInput::Buttons button)
		{
			switch(button)
			{
				case XboxInput::Buttons::A: return  0;
				case XboxInput::Buttons::B: return 1;
				case XboxInput::Buttons::X: return 2;
				case XboxInput::Buttons::Y: return 3;
				case XboxInput::Buttons::LeftUpperTrigger: return 4;
				case XboxInput::Buttons::RightUpperTrigger: return 5;
				case XboxInput::Buttons::Back: return 6;
				case XboxInput::Buttons::Start: return 7;
				case XboxInput::Buttons::LeftJoy: return 9;
				case XboxInput::Buttons::RightJoy: return 10;
				default: return -1;
			}
		}
		sf::Joystick::Axis translateAxis(XboxInput::Axis axis)
		{
			switch(axis)
			{
				case XboxInput::Axis::RightJoyX: return sf::Joystick::Axis::U;
				case XboxInput::Axis::RightJoyY: return sf::Joystick::Axis::V;
				case XboxInput::Axis::LeftJoyX: return sf::Joystick::Axis::X;
				case XboxInput::Axis::LeftJoyY: return sf::Joystick::Axis::Y;
				case XboxInput::Axis::DPadX: return sf::Joystick::Axis::PovX;
				case XboxInput::Axis::DPadY: return sf::Joystick::Axis::PovY;
				case XboxInput::Axis::LeftTrigger: return sf::Joystick::Axis::Z;
				case XboxInput::Axis::RightTrigger: return sf::Joystick::Axis::R;
			}	
		}

	// Otherwise assume windows
	#else

		bool usingWindows()
		{
			return true;
		}
		bool usingUnix()
		{
			return false;
		}

		unsigned int translateButton(XboxInput::Buttons button)
		{
			switch(button)
			{
				case XboxInput::Buttons::A: return  0;
				case XboxInput::Buttons::B: return 1;
				case XboxInput::Buttons::X: return 2;
				case XboxInput::Buttons::Y: return 3;
				case XboxInput::Buttons::LeftUpperTrigger: return 4;
				case XboxInput::Buttons::RightUpperTrigger: return 5;
				case XboxInput::Buttons::Back: return 6;
				case XboxInput::Buttons::Start: return 7;
				case XboxInput::Buttons::LeftJoy: return 8;
				case XboxInput::Buttons::RightJoy: return 9;
				default: return -1;
			}
		}
		sf::Joystick::Axis translateAxis(XboxInput::Axis axis)
		{
			switch(axis)
			{
				case XboxInput::Axis::RightJoyX: return sf::Joystick::Axis::U;
				case XboxInput::Axis::RightJoyY: return sf::Joystick::Axis::R;
				case XboxInput::Axis::LeftJoyX: return sf::Joystick::Axis:X;
				case XboxInput::Axis::LeftJoyY: return sf::Joystick::Axis::Y;
				case XboxInput::Axis::DPadX: return sf::Joystick::Axis::PovY;
				case XboxInput::Axis::DPadY: return sf::Joystick::Axis::PovX;
				case XboxInput::Axis::RightTrigger:
				case XboxInput::Axis::LeftTrigger: 
					return sf::Joystick::Axis::Z;
			}
		}
	#endif
	}
}
