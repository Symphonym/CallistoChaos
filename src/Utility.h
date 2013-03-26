#ifndef JL_UTILITY_H
#define JL_UTILITY_H

#include <sstream>
#include <string>
#include <typeinfo>
#include <stdexcept>

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
		template<typename T> T radToDeg(const T &radians)
		{
			return radians * (180/PI_VALUE);
		};
		template<typename T> T degToRad(const T &degrees)
		{
			return degrees * (PI_VALUE/180);
		};
	}
};

#endif