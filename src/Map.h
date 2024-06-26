#pragma once

#include "Tile.h"
#include <stdexcept>
#include <map>
#include <string>

class Update;

// enum Ore
// {
//     Air = 0,
//     // Copper = 1,
//     // Lead = 2,
//     Stone = 6,
//     Diamond = 7,
//     Redstone = 8
    
// };

// enum Liquid
// {
//     WaterUnder = 5,
//     WaterOver = 4,
//     Lava = 9

// };


class Map{

private:
        Tile** Tiles;
        unsigned int MAP_HEIGHT;
        unsigned int MAP_LENGTH;
        int GLOBAL_SEED;
        int SEA_LEVEL;
public:
        Map() : MAP_HEIGHT(100), MAP_LENGTH(200), GLOBAL_SEED(rand()){}
        Map(unsigned int map_height, unsigned int map_length,int global_seed) : MAP_HEIGHT(map_height), MAP_LENGTH(map_length), GLOBAL_SEED(global_seed)
        {
            Tiles = new Tile* [MAP_HEIGHT];
            for (int i=0; i != MAP_HEIGHT; i++)
            {
                Tiles[i] = new Tile [MAP_LENGTH];
            }
        }
        int GetGeneratedHeight(int X);
        int RandomWalkSurface();
        int GetMapHeight()
        {
            return MAP_HEIGHT;
        }
        int GetMapLength()
        {
            return MAP_LENGTH;
        }
        int GetSurfaceHeight(int X);
        Tile**& ReturnTiles(){
            return Tiles;
        }
        int LiquidStripe(const std::string& LiquidType,float UpperBoundary, float DownBoundary,float percentage);
        int PerlinCaves(const std::string& OreType);
        int PerlinHights(const std::string& OreType);
        int Walls();
        int Sides();
        ~Map(){
            for(int i=0; i!= MAP_HEIGHT;i++)
            {
                delete Tiles[i];
            }
            delete Tiles;
        }
        friend Update;
};