#ifndef BEDCONTROL_H
#define BEDCONTROL_H

#include <SFML/Graphics.hpp>

class Player;
class BedControl
{
private:

	Player *m_player;
	sf::Vector2i m_tileIndex;
	bool m_inBed;

public:
	explicit BedControl();

	void providePlayer(Player *player);
	void toggleBed(const sf::Vector2i &tileIndex = sf::Vector2i(0,0));
	bool isInUse() const;
};

#endif