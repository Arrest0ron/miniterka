#pragma once

#include <SFML/Graphics.hpp>
extern const int MAP_LENGTH;
extern int tileSize;

void ViewDynamics(sf::View& UserView)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && UserView.getSize().x < MAP_LENGTH*tileSize*2)
	{
		UserView.zoom(1.02);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && UserView.getSize().x > MAP_LENGTH*tileSize/100)
	{
		UserView.zoom(0.98);
	}
}