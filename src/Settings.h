#ifndef JL_SETTINGS_H
#define JL_SETTINGS_H

#include <map>
#include <string>

namespace jl
{
	namespace Settings
	{
		// Store data using a type-unique string ID 
		void setInt(const std::string& id, int value);
		void setDouble(const std::string& id, double value);
		void setBool(const std::string& id, bool value);
		void setString(const std::string& id, const std::string& value);

		// Retreive stored data by saved string ID, if the data cannot be found
		// under the specified ID; an exception will be thrown.
		int getInt(const std::string& id);
		double getDouble(const std::string& id);
		bool getBool(const std::string& id);
		std::string getString(const std::string& id);

	};
};

#endif