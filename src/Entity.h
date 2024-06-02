#pragma once
#include <SFML/Graphics.hpp>
#include <iostream> 

extern int tileSize;

class Entity;
class EntityStack;
class Player;
class Update;

extern void Still(const float BaseSpeed, Entity& ent,float K );
class Entity : public sf::Drawable
{
protected:

    int Health;
    int ModelHeight;
    int ModelLength;
    std::string name;
    std::vector<int> collision;
    bool facing;
    
    sf::Sprite m_sprite;
    

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
        // std::cout << name << " drawed. \n";
    }

    
    friend Update;
    friend Player;
    

public:
    sf::Clock LogicClock;
    void (*MOVE) (const float BaseSpeed,Entity& ent,float K);

    Entity(std::string Name= "",int modelLength = tileSize, int modelHeight = tileSize) : Health(100), ModelLength(modelLength), ModelHeight(modelHeight), movement(0,0), name(Name), facing(0) 
    {
        NullCollision();
        LogicClock.restart();
        MOVE = Still;
        std::cout << "Zero ent creation of " << Name << " .\n";

    }

    Entity(sf::Texture& texture, std::string Name= "",int modelLength = tileSize, int modelHeight = tileSize) : Health(100), ModelLength(modelLength), ModelHeight(modelHeight), movement(0,0), name(Name) 
    {
        NullCollision();
        LogicClock.restart();
        MOVE = Still;
        std::cout << "Entity created with texture and name " << Name << " .\n";

        this->setTexture(texture);
        this->GetSprite().setTextureRect(sf::IntRect(0,0,24,17));
    }

    ~Entity(){}

    sf::Vector2f movement;
    
    void setPosition(sf::Vector2f Pos)
    {
        m_sprite.setPosition(Pos.x,(Pos.y));
    }
    
    void setTexture(sf::Texture& texture)
    {
        m_sprite.setTexture(texture);
        m_sprite.setTextureRect(sf::IntRect(0,0,ModelLength,ModelHeight));
        
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

    void SetModelLength(int n)
    {
        ModelLength = n;
    }
    
    int GetModelHeight()
    {
        return ModelHeight;
    }
        
    void SetModelHeight(int n)
    {
        ModelHeight = n;
    }

    sf::FloatRect getGlobalBounds()
    {
        return m_sprite.getGlobalBounds();
    }
    
    sf::Sprite& GetSprite()
    {
        return m_sprite;
    }
    
    std::vector<int> GetCollision()
    {
        return collision;
    }
    
    int GetHealth()
    {
        return this->Health;
    }
    
    void AddHealth(int AddedHealth)
    {
        this->Health += AddedHealth;
    }
    void SetHealth(int Health)
    {
        this->Health = Health;
    }

    friend EntityStack;
};

class Player : public Entity
{
private:
    unsigned score;
public:
    Player() : Entity(){}
    int GetScore() const {return this->score;}
    void SetScore(const int n) {this->score = n;}
    void IncreaseScore(const int n) {this->score += n;}
};

class EntityStack
{
    Entity* entityStack;
    inline static int MaxEntityAmount = 1;
    inline static int amount = 0;
public:

    EntityStack() 
    {
        entityStack = new Entity [MaxEntityAmount];
    }
    ~EntityStack()
    {
        if (entityStack != nullptr)
        {
            delete entityStack;
        }
        
    }
    void CreateEntity(int ModelWidth, int ModelHeight, sf::Texture& EntityTexture, std::string name = std::to_string(amount))
    {
        Entity* NewEntity = new Entity(EntityTexture,name,24,17);
        // NewEntity->m_sprite.setTextureRect(sf::IntRect(0,0,24,17));

        entityStack[amount] = *NewEntity;
        amount++;
    }  
    void CreateEntity(int ModelWidth, int ModelHeight, std::string name = std::to_string(amount))
    {
        Entity* NewEntity = new Entity(name,24,17);
        // NewEntity->m_sprite.setTextureRect(sf::IntRect(0,0,24,17));

        entityStack[amount] = *NewEntity;
        amount++;
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



