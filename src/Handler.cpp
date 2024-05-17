#include "Handler.h"    
#include <SFML/Graphics.hpp>
#include <iostream>   


int const TILESET_SIZE = 32*32;
int TILESET_X = 32;
int tileSize = 16; // Размер каждого тайла



sf::Texture TextureSetCreation(const std::string& texture)
{
    // Загрузка тайлсета
    sf::Texture tileset;
    if (!tileset.loadFromFile(texture))
    {
        std::cerr << "Failed to load tileset!" << std::endl;
    }

    return tileset;
}

sf::Sprite* TileSetCreation(sf::Texture tileset)
{


    // Создание спрайтов для каждого тайла из тайлсета
    sf::Sprite* tiles = new sf::Sprite [TILESET_SIZE];

        // Установка текстур с тайлсета в тайлы
    for (int i = 0; i < TILESET_SIZE; ++i)
    {
        int x = (i % TILESET_X) * tileSize;
        int y = (i / TILESET_X) * tileSize;
        tiles[i].setTexture(tileset);
        tiles[i].setTextureRect(sf::IntRect(x, y, tileSize, tileSize));
    }
    return tiles;
}