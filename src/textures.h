#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <iostream>
#include <cmath>
#include <ctime>

void applyPerlinNoiseInsideStones(int**& map, int MAP_LENGTH, int MAP_HEIGHT, int SEALEVEL);
void PerlinOre(int**& map, int MAP_LENGTH, int MAP_HEIGHT, int SEALEVEL);
double perlin(int x, int y);

float smoothNoise(float x, float y);
float interpolate(float a, float b, float x);
float interpolatedNoise(float x, float y);

enum Ore{
    Air = 0,
    Copper = 1,
    Lead = 2,
    Diamond = 3,
    Redstone = 4
};

class Tile{

private: 
    

    int TextureID;
    int Type;

public:
    Tile(int ID,int type): TextureID(ID), Type(type){}
    Tile(int ID): TextureID(ID), Type(0){}
    Tile(): TextureID(0), Type(0){}

    void operator=(int n)
    {
        TextureID = n;
    }
    void SetType(int n)
    {
        Type = n;
    }
    int GetTile(){
        return TextureID;
    }
    bool operator==(int N)
    {
        return TextureID==N;
    }

    ~Tile(){}
};




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
        int RandomWalkSurface();
        
        // int OceanGen();
        // int LakesGen();
        int PerlinCaves(Ore OreType);
        // int PerlinOres();
        // int GrassTiles();

        Tile* operator[](int Y)
        {
            if (Y<0 || Y >= MAP_HEIGHT){
                throw std::runtime_error("Y out of bounds.");
            }
            return Tiles[Y];
        }


        ~Map(){
            for(int i=0; i!= MAP_HEIGHT;i++)
            {
                delete Tiles[i];
            }
            delete Tiles;
        }
};


// class Update{
//     private:
//         Map tilemap;
//         void UpdateWater();
//         void UpdateFallingTile();
//         // void 
// };