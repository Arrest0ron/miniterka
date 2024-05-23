#pragma once
#include "Entity.h"
#include "Map.h"

class EntityStack;
class Entity;
class Player;


class Update
{
private:
    Map& tilemap;
    EntityStack& Entities;
    int EntitiesMAX;

    void UpdateLiquids(std::vector<int>& Loaded);
    void UpdateFallingTile();
    void UpdateEntities();
    void Collision(Entity& ent);
    void Direction(Entity& ent);
public:

    Update(Map& tiles, EntityStack& EntityList, int entMAX, Player& user) : tilemap(tiles), Entities(EntityList), EntitiesMAX(entMAX){}
    void tick(std::vector<int>& Loaded)
    {
        UpdateLiquids(Loaded);
        UpdateEntities();
    }
    void UpdatePlayer(Player& User);
    ~Update(){}     
};