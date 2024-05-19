#include "Handler.h"    
#include <SFML/Graphics.hpp>
#include <iostream>   

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

std::vector<float> GetBlocksAroundCoordinatesFloat(sf::FloatRect& Bounds)
{ 
    std::vector<float> BlocksAroundCoordinates  // top,down,left,right
    {
    Bounds.top, Bounds.top + Bounds.height,
    Bounds.left,Bounds.left+ Bounds.width 
    };
    return BlocksAroundCoordinates;
}

std::vector<int>   GetBlocksAroundCoordinatesInt(sf::FloatRect& Bounds)
{
    
    std::vector<float> BlocksAroundCoordinates = GetBlocksAroundCoordinatesFloat(Bounds);
    std::vector<int>   BlocksAroundCoordinatesInt  // top,down,left,right
    {
    static_cast<int>(BlocksAroundCoordinates[0]/16), static_cast<int>(BlocksAroundCoordinates[1]/16),
    static_cast<int>(BlocksAroundCoordinates[2]/16), static_cast<int>(BlocksAroundCoordinates[3]/16)
    };
    return BlocksAroundCoordinatesInt;
}

std::vector<float> GetEntityCoordinatesFloat(sf::FloatRect& Bounds)
{ 
    std::vector<float> EntityCoordinates  // top,down,left,right
    {
    Bounds.top+1, Bounds.top  + Bounds.height-2,
    Bounds.left+1,Bounds.left + Bounds.width-1
    };
    return EntityCoordinates;
}

std::vector<int>   GetEntityCoordinatesInt(sf::FloatRect& Bounds)
{

    std::vector<float> EntityCoordinates = GetEntityCoordinatesFloat(Bounds);
    std::vector<int> EntityCoordinatesInt  // top,down,left,right
    {
    static_cast<int>(EntityCoordinates[0]/16), static_cast<int>(EntityCoordinates[1]/16),
    static_cast<int>(EntityCoordinates[2]/16), static_cast<int>(EntityCoordinates[3]/16)
    };
    return EntityCoordinatesInt;
}

// void TileSetCreation(sf::Sprite* tiles, sf::Texture tileset)
// {


//     // Создание спрайтов для каждого тайла из тайлсета
//     tiles = new sf::Sprite [TILESET_SIZE];

//         // Установка текстур с тайлсета в тайлы
//     for (int i = 0; i < TILESET_SIZE; ++i)
//     {
//         int x = (i % TILESET_X) * tileSize;
//         int y = (i / TILESET_X) * tileSize;
//         tiles[i].setTexture(tileset);
//         tiles[i].setTextureRect(sf::IntRect(x, y, tileSize, tileSize));
//     }
// }

