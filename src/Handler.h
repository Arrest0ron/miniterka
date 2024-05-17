#pragma once
#include <SFML/Graphics.hpp>

sf::Texture TextureSetCreation(const std::string& texture);

void TileSetCreation(sf::Sprite* tiles, sf::Texture tileset);