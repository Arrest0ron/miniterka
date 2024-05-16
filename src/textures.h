#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <memory>
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
<<<<<<< HEAD


double perlin(int x, int y);
float smoothNoise(float x, float y);
float interpolate(float a, float b, float x);
float interpolatedNoise(float x, float y);
=======


>>>>>>> 5b851c13997a0d7472366e016260d8e236a3b7c4

#include "Tile.h"
#include "Map.h"
#include "Update.h"

class Update;

class Map;
class Tile;
class Entity;
<<<<<<< HEAD
class Map;
class Tile;
class EntityStack;
class Player;

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
        int LiquidStripe(Liquid LiquidType,float UpperBoundary, float DownBoundary,float percentage);
        int PerlinCaves(Ore OreType);
        int PerlinHights(Ore OreType);
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


class Entity : public sf::Drawable
{
private:

    int Health;
    int ModelHeight;
    int ModelLength;
    std::vector<bool> collision;
    int facing;
    

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
    }

    sf::Sprite m_sprite;
    friend Update;
    friend Player;
    

public:

    Entity() : Health(100), ModelLength(16), ModelHeight(16), movement(0,0) 
    {
        NullCollision();
        std::cout << "entity created. \n";
    }

    Entity(sf::Texture texture)
    {
        this->setTexture(texture);
    }
    ~Entity(){}
    sf::Vector2f movement;
    void setPosition(sf::Vector2f Pos)
    {
        m_sprite.setPosition(Pos.x,(Pos.y));
    }
    void setTexture(sf::Texture texture)
    {
        m_sprite.setTexture(texture);
        m_sprite.setTextureRect(sf::IntRect(32,0,ModelLength,ModelHeight));
        
    }
    void NullCollision()
    {
        for (int i=0;i!=4;i++)
        {
            collision.push_back(0);
        }
    }
    int GetModelLength()
    {
        return ModelLength;
    }
    int GetModelHeight()
    {
        return ModelHeight;
    }
    sf::FloatRect getGlobalBounds()
    {
        return m_sprite.getGlobalBounds();
    }
    sf::Sprite& GetSprite()
    {
        return m_sprite;
    }
    std::vector<bool> GetCollision()
    {
        return collision;
    }

    friend EntityStack;
};
class Player : public Entity
{

};

class EntityStack
{
    Entity* entityStack;
    int MaxEntityAmount;
public:
    EntityStack(int amount, Entity*& stack) : MaxEntityAmount(amount), entityStack(stack){};
    ~EntityStack()
    {
        delete entityStack;
    }
    void operator=(EntityStack& Stack)
    {
        this->entityStack = Stack.entityStack;
    }
    Entity& operator[](int num)
    {
        if (num>=MaxEntityAmount)
        {
            std::cout << "EntityStack[>MaxSize]";
            return entityStack[0];
        }
        return entityStack[num];
    }
    friend Update;
};



class Update{
    private:
        Map tilemap;
        EntityStack Entities;
        int EntitiesMAX;

        void UpdateLiquids();
        void UpdateFallingTile();
        void UpdateEntities();
        
        void Collision(Entity&);
    public:

        Update(Map& tiles, EntityStack& EntityList, int entMAX, Player& user) : tilemap(tiles), Entities(EntityList), EntitiesMAX(entMAX){}
        void tick()
        {
            UpdateLiquids();
            UpdateEntities();
        }
        void UpdatePlayer(Player& User);
        ~Update(){}
        
};
=======
class EntityStack;
class Player;









>>>>>>> 5b851c13997a0d7472366e016260d8e236a3b7c4
