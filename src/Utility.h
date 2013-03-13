#ifndef CC_UTILITY_H
#define CC_UTILITY_H

#include <sstream>
#include <string>
#include <typeinfo>
#include <stdexcept>

namespace cc
{
	namespace Util
	{
		// Attempts to convert between datatypes using std::stringstream,  convert from <this, to this>
		// Only works on built-in types, std::string included
		template<typename Tin, typename Tout> Tout convertData(const Tin& input)
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
	};

	namespace Math
	{
		// Clamps a value to be >= min and <= max
		template<typename T, typename Tmin, typename Tmax> T clamp(const T &value, const Tmin &min, const Tmax &max)
		{
			return value < min ? min : (value > max ? max : value);
		};
	}
};

#endif