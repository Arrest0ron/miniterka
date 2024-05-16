#pragma once
#include "Entity.h"
#include "Map.h"

class EntityStack;
class Entity;
class Player;


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