#pragma once
#include <SFML/Graphics.hpp>
#include <iostream> 

class Entity;
class EntityStack;
class Player;
class Update;

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



