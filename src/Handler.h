#pragma once
#include <SFML/Graphics.hpp>

sf::Texture TextureSetCreation(const std::string& texture);

void TileSetCreation(sf::Sprite* tiles, sf::Texture tileset);

std::vector<float> GetBlocksAroundCoordinatesFloat(sf::FloatRect& Bounds);
std::vector<int> GetBlocksAroundCoordinatesInt(sf::FloatRect& Bounds);
std::vector<float> GetEntityCoordinatesFloat(sf::FloatRect& Bounds);
std::vector<int> GetEntityCoordinatesInt(sf::FloatRect& Bounds);