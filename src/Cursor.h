#pragma once

#include "Entity.h"
#include <SFML/Graphics.hpp>

class Cursor
{
	Player& Owner;
	sf::Vector2f Pos;
public:
	sf::Vector2f DistanceFromOwner()
	{
		return sf::Vector2f(Owner.getGlobalBounds().top + Owner.getGlobalBounds().height/2, Owner.getGlobalBounds().left + Owner.getGlobalBounds().width/2);
	}
	void UpdatePos(sf::RenderWindow window)
	{
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		Pos = window.mapPixelToCoords(pixelPos);
		return;
	}
};