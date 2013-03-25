#include "MessageLog.h"

namespace MessageLog
{
	std::vector<std::pair<std::string, double>> m_messages;
	sf::Text m_text;

	void addMessage(const std::string &message)
	{
		m_messages.push_back(std::make_pair(message, 0));

		// Limit log to 5 messages
		if(m_messages.size() > 5)
			m_messages.erase(m_messages.begin());
	}
	void loadAssets(jl::AssetManager &assets)
	{
		m_text.setFont(assets.getAsset<jl::FontAsset>("res/Minecraftia.ttf")->get());
		m_text.setCharacterSize(12);
		m_text.setPosition(0,0);
	}

	void update(double deltaTime)
	{
		for(std::size_t i = 0; i < m_messages.size(); i++)
		{
			m_messages[i].second += deltaTime;

			if(m_messages[i].second >= 5.0)
			{
				m_messages.erase(m_messages.begin() + i);
				continue;
			}
		}
	}
	void render(sf::RenderTarget &target)
	{
		for(std::size_t i = 0; i < m_messages.size(); i++)
		{
			m_text.setString(m_messages[i].first);
			m_text.setPosition(0, i * m_text.getGlobalBounds().height);

			sf::Color color = sf::Color::White;
			color.a = 255 - m_messages[i].second * 50;
			m_text.setColor(color);

			sf::View tempView(target.getDefaultView());
			target.setView(target.getDefaultView());

			target.draw(m_text);
			target.setView(tempView);
		}
	}
};