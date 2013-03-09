#include "Settings.h"
#include <stdexcept>

namespace cc
{

	void Settings::setInt(const std::string& id, int value){ m_ints[id] = value; }
	void Settings::setDouble(const std::string& id, double value){ m_doubles[id] = value; }
	void Settings::setBool(const std::string& id, bool value){ m_bools[id] = value; }
	void Settings::setString(const std::string& id, const std::string& value){ m_strings[id] = value; }

	int Settings::getInt(const std::string& id)
	{
		if(m_ints.find(id) != m_ints.end())
			return m_ints[id];
		else
			throw std::runtime_error("Integer setting: \"" + id + "\" not found");
	}
	double Settings::getDouble(const std::string& id)
	{
		if(m_doubles.find(id) != m_doubles.end())
			return m_doubles[id];
		else
			throw std::runtime_error("Double setting: \"" + id + "\" not found");
	}
	bool Settings::getBool(const std::string& id)
	{
		if(m_bools.find(id) != m_bools.end())
			return m_bools[id];
		else
			throw std::runtime_error("Bool setting: \"" + id + "\" not found");
	}
	std::string Settings::getString(const std::string& id)
	{
		if(m_strings.find(id) != m_strings.end())
			return m_strings[id];
		else
			throw std::runtime_error("String setting: \"" + id + "\" not found");
	}

};