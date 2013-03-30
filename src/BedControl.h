#ifndef BEDCONTROL_H
#define BEDCONTROL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player;
class BedControl
{
private:

	Player *m_player;
	bool m_inBed;

	double m_regenDelay;
	sf::Clock m_regenClock;

public:
	explicit BedControl(Player *player);

	void update();

	void setRegenDelay(double secondDelay);

	void toggleBed(const sf::Vector2i &tileIndex = sf::Vector2i(0,0));
	bool isInUse() const;
};

#endif