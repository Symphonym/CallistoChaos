#include "SettingsConsole.h"
#include "Utility.h"
#include "GameState.h"
#include <sstream>

namespace SettingsConsole
{
	sf::Color m_backgroundColor = sf::Color(0,0,0, 100);
	bool m_displayConsole = false;
	double m_textHeight = 0.0;
	sf::Text m_text;

	void setColor(const sf::Color &color)
	{
		m_backgroundColor = color;
	}
	void setFontData(const sf::Font &font, int fontSize)
	{
		m_text.setFont(font);
		m_text.setCharacterSize(fontSize);
		m_text.setString("|");

		m_textHeight = m_text.getGlobalBounds().height;
		m_text.setString("");
	}

	void events(sf::Event &events, GameState &gameState)
	{	
		// Bring up console
		if(jl::Input::isKeyDown(events, sf::Keyboard::F1))
			m_displayConsole = !m_displayConsole;

		if(m_displayConsole)
		{
			// Commiting updates
			if(jl::Input::isKeyDown(events, sf::Keyboard::Return))
			{
				std::vector<std::string> consoleArgs;
				std::stringstream ss(m_text.getString());

				std::string arg = "";
				bool modifyWindow = false;
				while(ss >> arg)
				{
					if(arg.find("window") != std::string::npos)
						modifyWindow = true;

					consoleArgs.push_back(arg);
				}

				gameState.getEngine()->parseArgs(consoleArgs);

				if(modifyWindow)
				{
					gameState.getEngine()->refreshSettings();
					gameState.reloadView();
				}

				m_text.setString("");
			}

			// Text input
			else if (events.type == sf::Event::TextEntered)
			{
			    // Handle ASCII characters only
			    if (events.text.unicode < 128)
			    {
			    	 std::string consoleInput(m_text.getString());

			    	// Check for delete or backspace characters AND carriage return and line feed
			    	if(events.text.unicode == 128 || events.text.unicode == 8 || events.text.unicode == 13 || events.text.unicode == 10)
			    	{
			    		if(consoleInput.size() > 0)
			    			m_text.setString(consoleInput.substr(0, consoleInput.size() - 1));
					}
			    	else
			    	{
				        consoleInput += static_cast<char>(events.text.unicode);
				        m_text.setString(consoleInput);
				    }
			    }
			}
		}
	}
	void render(sf::RenderTarget &target)
	{

		if(m_displayConsole)
		{

			sf::View tempView(target.getView());
			target.setView(target.getDefaultView());


			sf::Image background;
			background.create(target.getView().getSize().x, m_textHeight+5, m_backgroundColor);

			sf::Texture backgroundTexture;
			backgroundTexture.loadFromImage(background);

			sf::Sprite backgroundSprite(backgroundTexture);
			target.draw(backgroundSprite);

			m_text.setPosition(10, 2);
			target.draw(m_text);


			target.setView(tempView);
		}
	}
};	