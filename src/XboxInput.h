#ifndef JL_XBOXINPUT_H
#define JL_XBOXINPUT_H

#include <SFML/Graphics.hpp>

namespace jl
{
	namespace XboxInput
	{
		enum Axis
		{
			RightJoyX,
			RightJoyY,
			LeftJoyX,
			LeftJoyY,
			DPadX,
			DPadY,
			LeftTrigger,
			RightTrigger
		};

		enum Buttons
		{
			A,
			B,
			X,
			Y,
			LeftUpperTrigger,
			RightUpperTrigger,
			RightJoy,
			LeftJoy,
			Start,
			Back
		};

		bool isAxisDown(unsigned int controllerIndex, XboxInput::Axis axis, float triggerValue, float valueSpread = 25);
		bool isButtonDown(unsigned int controllerIndex, XboxInput::Buttons button);

		sf::Joystick::Axis translateAxis(XboxInput::Axis axis);
		unsigned int translateButton(XboxInput::Buttons button);
	};
};

#endif