#pragma once

#include "Entity.h"
#include "Map.h"
#include <SFML/Graphics.hpp>
#include <cmath>

extern int tileSize;
class Cursor
{
	Player& Owner;
	Map& map;
	sf::Vector2f Pos;
public:
	Cursor(Player& User, Map& MapGiven) : Owner(User), map(MapGiven) {}
	float DistanceFromOwner()
	{
		return pow(pow((Owner.getGlobalBounds().top + Owner.getGlobalBounds().height/2 - Pos.y),2) +  pow((Owner.getGlobalBounds().left + Owner.getGlobalBounds().width/2 - Pos.x),2),0.5);
	}
	void UpdatePos(sf::RenderWindow& window)
	{
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		Pos = window.mapPixelToCoords(pixelPos);
		return;
	}
	sf::Vector2i BlockTouched()
	{
		sf::Vector2i tilePos(static_cast<int>(Pos.x) / tileSize, static_cast<int>(Pos.y) / tileSize);
    	return tilePos;
	}
};