#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <memory>
#include <iostream>
#include <cmath>
#include <ctime>

void applyPerlinNoiseInsideStones(int**& map, int MAP_LENGTH, int MAP_HEIGHT, int SEALEVEL);
void PerlinOre(int**& map, int MAP_LENGTH, int MAP_HEIGHT, int SEALEVEL);
double perlin(int x, int y);

float smoothNoise(float x, float y);
float interpolate(float a, float b, float x);
float interpolatedNoise(float x, float y);


class Update;
class Entity;

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
        bool operator!=(int N)
    {
        return TextureID!=N;
    }

    ~Tile(){}
};

class Update;

// class Entity
// {
//  private:
//     int EntityID;
//     int Health;
//     sf::Time OutOfView;
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
        int GetSurfaceHeight(int X);
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
        std::unique_ptr<Entity[]> Entities;
        int EntitiesMAX;
        void UpdateLiquids();
        void UpdateFallingTile();
        void UpdateEntities();
    public:
        Update(Map& tiles, Entity* EntityList, int entMAX) : tilemap(tiles), Entities(EntityList), EntitiesMAX(entMAX){}
        void tick()
        {
            UpdateLiquids();
            // UpdateEntities();
        }
        ~Update(){}
        
};

class Entity : public sf::Drawable
{
public:
    Entity() : Health(100), ModelLength(16), ModelHeight(16) {}
    Entity(sf::Texture texture) : m_texture(texture){}
    ~Entity(){}
    void setPosition(sf::Vector2f Pos)
    {
        m_sprite.setPosition(Pos.x,(Pos.y-1));
    }
    void setTexture(sf::Texture texture)
    {
        m_texture = texture;
        m_sprite.setTexture(m_texture);
        m_sprite.setTextureRect(sf::IntRect(32,0,ModelLength,ModelHeight));
    }
    int GetModelLength()
    {
        return ModelLength;
    }
    int GetModelHeight()
    {
        return ModelHeight;
    }

    sf::Sprite& GetSprite()
    {
        return m_sprite;
    }

    float top = m_sprite.getGlobalBounds().top;
    float down = top + m_sprite.getGlobalBounds().height;
    float left = m_sprite.getGlobalBounds().left;
    float right = left + m_sprite.getGlobalBounds().width;
private:
    int Health;
    int ModelHeight;
    int ModelLength;




    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
    }

    sf::Sprite m_sprite;
    sf::Texture m_texture;
    friend Update;
};