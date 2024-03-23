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

enum Ore
{
    Air = 0,
    // Copper = 1,
    // Lead = 2,
    Diamond = 7,
    Redstone = 8,
    Stone = 6
};

enum Liquid
{
    WaterUnder = 5,
    WaterOver = 4,
    Lava = 9

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
    void SetID(int n)
    {
        TextureID=n;
    }

    void SetType(int n)
    {
        Type = n;
    }
    int GetTile(){
        return TextureID;
    }
    int GetType()
    {
        return Type;
    }

    bool operator==(int N)
    {
        return TextureID==N;
    }

    ~Tile(){}
};

class Update;

class Entity
{
 private:
    int EntityID;
    int Health;
    sf::Time OutOfView;
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
        int GetGeneratedHeight(int X)
        {
            int EncounteredSurface = 0;
            for(int Y=0;Y != MAP_HEIGHT;Y++)
            {
                
                if (Tiles[Y][X]==6)
                {
                    EncounteredSurface = 1;
                }
                if (EncounteredSurface && Tiles[Y][X] == 0)
                {
                    return Y;
                }
            }
            return MAP_HEIGHT-1;
        }

        int RandomWalkSurface();
        int GetMapHeight()
        {
            return MAP_HEIGHT;
        }
        int GetMapLength()
        {
            return MAP_LENGTH;
        }
        Tile**& ReturnTiles(){
            return Tiles;
        }
        int LiquidStripe(Liquid LiquidType,float UpperBoundary, float DownBoundary,float percentage);
        // int OceanGen();
        // int LakesGen();
        int PerlinCaves(Ore OreType);
        int PerlinHights(Ore OreType);
        // int 
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
        friend Update;
};

class Update{
    private:
        Map tilemap;
        void UpdateLiquids();
        void UpdateFallingTile();
        // void UpdateEntities();
    public:
        Update(Map& tiles) : tilemap(tiles){}
        void tick()
        {
            UpdateLiquids();
        }
        
};
