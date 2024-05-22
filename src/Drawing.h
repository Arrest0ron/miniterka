#pragma once

#include <SFML/Graphics.hpp>
extern int tileSize;
template<class T>
void DrawText(sf::RenderWindow& window,sf::Font& font,float x,float y, T Text,int size = tileSize, sf::Color Color = sf::Color::White)
{
	sf::Text DrawableText;
	DrawableText.setFont(font);
	DrawableText.setCharacterSize(tileSize);
	DrawableText.setPosition(x, y);
	DrawableText.setString(std::to_string(Text));
	DrawableText.setFillColor(Color);
	window.draw(DrawableText);
}
void DrawText(sf::RenderWindow& window,sf::Font& font,float x,float y, std::string Text,int size = tileSize, sf::Color Color = sf::Color::White)
{
	sf::Text DrawableText;
	DrawableText.setFont(font);
	DrawableText.setCharacterSize(tileSize);
	DrawableText.setPosition(x, y);
	DrawableText.setString(Text);
	DrawableText.setFillColor(Color);
	window.draw(DrawableText);
}
void DrawContainingBox(sf::RenderWindow& window, Entity& ent)
{
	sf::FloatRect Bounds = ent.getGlobalBounds();
	std::vector<float> EntityCoordinatesFloat = GetEntityCoordinatesFloat(Bounds);

	sf::Vertex vertices[4] =
	{
		sf::Vertex(sf::Vector2f(EntityCoordinatesFloat[2],EntityCoordinatesFloat[0])),
		sf::Vertex(sf::Vector2f(EntityCoordinatesFloat[3],EntityCoordinatesFloat[0])),
		sf::Vertex(sf::Vector2f(EntityCoordinatesFloat[2],EntityCoordinatesFloat[1])),
		sf::Vertex(sf::Vector2f(EntityCoordinatesFloat[3],EntityCoordinatesFloat[1]))

	};
	window.draw(vertices, 4, sf::Quads);
}
void DrawContainingBoxInt(sf::RenderWindow& window, Entity& ent)
{
	sf::FloatRect Bounds = ent.getGlobalBounds();
	std::vector<int> EntityCoordinatesInt = GetEntityCoordinatesInt(Bounds);

	sf::Vertex vertices[4] =
	{
		sf::Vertex(sf::Vector2f(EntityCoordinatesInt[2]*tileSize,EntityCoordinatesInt[0]*tileSize)),
		sf::Vertex(sf::Vector2f(EntityCoordinatesInt[3]*tileSize,EntityCoordinatesInt[0]*tileSize)),
		sf::Vertex(sf::Vector2f(EntityCoordinatesInt[2]*tileSize,EntityCoordinatesInt[1]*tileSize)),
		sf::Vertex(sf::Vector2f(EntityCoordinatesInt[3]*tileSize,EntityCoordinatesInt[1]*tileSize))

	};
	window.draw(vertices, 4, sf::Quads);
}
sf::Color gradientRG(int maxNum, int t)
{
	int fromR  = 255;
	int fromG  = 0;
	int fromB  = 0;

	int toR    = 0;
	int toG    = 255;
	int toB    = 0;

	int deltaR = static_cast<int>((toR - fromR) / maxNum);
	int deltaG = static_cast<int>((toG - fromG) / maxNum);
	int deltaB = static_cast<int>((toB - fromB) / maxNum);

	int R      = fromR + t * deltaR;
	int G      = fromG + t * deltaG;
	int B      = fromB + t * deltaB;
	return sf::Color(R,G,B);
}