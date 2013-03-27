#ifndef JL_UTILITY_H
#define JL_UTILITY_H

#include <sstream>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include <SFML/Graphics.hpp>

#define PI_VALUE 3.14159265359

namespace jl
{
	namespace Util
	{
		// Attempts to convert between datatypes using std::stringstream,  convert from <this, to this>
		// Only works on built-in types, std::string included
		template<typename Tin, typename Tout> Tout convertData(Tin input)
		{
			std::stringstream ss;
			ss << input;

			Tout data;
			ss >> data;

			// Throw exception if invalid input was given
			if(ss.fail())
				throw std::runtime_error("The value \"" + input + "\" could not be converted to the specified datatype properly.");
			
			return data;
		};
		template<typename T> std::string toString(const T &input)
		{
			std::stringstream ss;
			ss << input;

			return ss.str();
		};
	};

	namespace Math
	{
		// Clamps a value to be >= min and <= max
		template<typename T, typename Tmin, typename Tmax> T clamp(const T &value, const Tmin &min, const Tmax &max)
		{
			return value < min ? min : (value > max ? max : value);
		};

		// Converts radians to degrees
		template<typename T> T radToDeg(const T &radians)
		{
			return radians * (180/PI_VALUE);
		};

		// Converts degrees to radians
		template<typename T> T degToRad(const T &degrees)
		{
			return degrees * (PI_VALUE/180);
		};

		// Checks if value is within maxValue +- range
		template<typename T, typename Tmiddle, typename Trange> bool valueInRange(const T &value, const Tmiddle &maxValue, const Trange &range)
		{
			return value <= (maxValue + range) ? (value >= (maxValue - range) ? true : false) : false;
		};
	};

	namespace Vec
	{
		// Linear interpolation between start and end
		inline sf::Vector2f lerp(const sf::Vector2f &start, const sf::Vector2f &end, double percent)
		{
			return sf::Vector2f(start + sf::Vector2f(percent*(end.x - start.x),percent*(end.y - start.y)));
		};
	};

	namespace Input
	{
		inline bool isButtonDown(const sf::Event &events, unsigned int buttonIndex)
		{
			return events.type == sf::Event::JoystickButtonPressed && events.joystickButton.button == buttonIndex;
		};
		inline bool isKeyDown(const sf::Event &events, sf::Keyboard::Key key)
		{
			return events.type == sf::Event::KeyPressed && events.key.code == key;
		};
		inline bool isAxisDown(const sf::Event &events, sf::Joystick::Axis axis, double value)
		{
			return events.type == sf::Event::JoystickMoved && events.joystickMove.axis == axis && Math::valueInRange<double, double,double>(events.joystickMove.position, value, 25);
		};
		inline int scrollDirection(const sf::Event &events)
		{
			return events.type == sf::Event::MouseWheelMoved ? (events.mouseWheel.delta > 0 ? 1 : -1) : 0;
		};
	};
};

#endif