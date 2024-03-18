#include "textures.h"

//генерация массива
int** GenerateArray(int width, int height, bool empty)
{
    int** map = new int* [width];
    for(int x = 0; x < width; x++){
        map[x] = new int[height];
        for(int y = 0; y < height; y++)
        {
            if(empty)
            {
                map[x][y] = 0;
            } 
            else
            {
                map[x][y] = 1;
            }
        }
    }
    return map;
}

// рендеринг карты
void RenderMap(int** map, int width, int height, sf::RenderWindow& window, sf::Texture& texture, int tileSize)
{
    window.clear();
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            if (map[x][y] = 1)
            {
                sf::Sprite tile(texture);
                tile.setPosition(x * tileSize, y * tileSize);
                window.draw(tile);
            }
        }
    }
}

// void UpdateMap(int** map, Tilemap tilemap)
// {
//     for(int x = 0; x < width; x++)
//     {

//     }
// }