#ifndef SETTINGS_H
#define SETTINGS_H

#include <map>
#include <string>

namespace cc
{
	class Settings
	{
	private:
		std::map<std::string, int> m_ints;
		std::map<std::string, double> m_doubles;
		std::map<std::string, bool> m_bools;
		std::map<std::string, std::string> m_strings;
		
	public:

		void setInt(const std::string& id, int value);
		void setDouble(const std::string& id, double value);
		void setBool(const std::string& id, bool value);
		void setString(const std::string& id, const std::string& value);

		int getInt(const std::string& id);
		double getDouble(const std::string& id);
		bool getBool(const std::string& id);
		std::string getString(const std::string& id);

	};
};

#endif