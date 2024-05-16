#include "Update.h"
#include "Entity.h"
#include <vector>


// битовая переменная нужна для прихода к положительному значения, а double для прихода к значению от -1 до 1, хз зачем, так в алгоритмах пишут, что это стандарт. остальные числа для хаотичности

void Update::UpdateLiquids()
{
    int MapHeight = tilemap.GetMapHeight();
    int MapLength = tilemap.GetMapLength();
    for (int Y=0; Y!=MapHeight;Y++)
    {
        for (int X=0;X!=MapLength;X++)
        {
            if (tilemap.Tiles[Y][X].GetType() == 1)
            {
                if (Y < MapHeight-1 && tilemap[Y+1][X] == 0)
                {
                    std::swap(tilemap[Y][X], tilemap[Y+1][X]);
                }
                switch (rand()%2) 
                {
                case 0:
                    if (X > 0 && tilemap[Y][X-1] == 0)
                    {
                        std::swap(tilemap[Y][X], tilemap[Y][X-1]);
                    }
                    break;
                case 1:            
                    if (X < MapLength && tilemap[Y][X+1] == 0)
                    {
                        std::swap(tilemap[Y][X], tilemap[Y][X+1]);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void Update::UpdateEntities()
{
    for (int i = 0; i!=EntitiesMAX;i++)
    {
        Collision(Entities[i]);
    }
}

void Update::Collision(Entity& ent)
{
    float topBlock = ent.getGlobalBounds().top-1;
    float downBlock = topBlock + ent.getGlobalBounds().height+2;
    float leftBlock = ent.getGlobalBounds().left-1;
    float rightBlock = leftBlock + ent.getGlobalBounds().width+2;
    int topBlockI = topBlock/16;
    int downBlockI = downBlock/16;
    int leftBlockI = leftBlock/16;
    int rightBlockI = rightBlock/16;

    float topEntity = ent.getGlobalBounds().top;
    float downEntity = topEntity + ent.getGlobalBounds().height;
    float leftEntity = ent.getGlobalBounds().left;
    float rightEntity = leftEntity + ent.getGlobalBounds().width;
    int topEntityI = (topEntity)/16;
    int downEntityI = (downEntity)/16;
    int leftEntityI = (leftEntity)/16;
    int rightEntityI = (rightEntity)/16;




    std::vector <bool> flag{0,0,0,0};
    
    for (int X = leftEntityI; X != rightEntityI+1;X++) 
    {
        // if (downBlockI==(tilemap.GetMapHeight()-1))
        // {
        //     flag[0] = 1;
        //     break;
        // }
        int IDCheck = tilemap[downBlockI][X].GetTile();
        if (IDCheck != 0)
        {
            flag[0] = 1;
            break;
        }
    }

    ent.collision[0] = flag[0];

    for (int X = leftEntityI; X != rightEntityI+1;X++) 
    {
        // if (topBlockI)
        // {
            // flag[1] = 1;
            // break;
        // }
        int IDCheck = tilemap[topBlockI][X].GetTile();
        if (IDCheck != 0)
        {
            flag[1] = 1;
            break;
        }
    }
    ent.collision[1] = flag[1];

    
    for (int Y = downEntityI; Y != topEntityI-1;Y--) 
    {
        // if (!static_cast<int>((ent.getGlobalBounds().left)/16+1))
        // {
            // flag[2] = 1;
            // break;
        // }
        int IDCheck = tilemap[Y][leftBlockI].GetTile();
        if (IDCheck != 0)
        {
            flag[2] = 1;
            break;
        }
    }
    ent.collision[2] = flag[2];

    
    for (int Y = downEntityI; Y != topEntityI-1;Y--) 
    {
        // if (rightBlock >= tilemap.GetMapLength())
        // {
            // flag[3] = 1;
            // break;
        // }
        int IDCheck = tilemap[Y][rightBlockI].GetTile();
        if (IDCheck != 0)
        {
            flag[3] = 1;
            break;
        }
    }
    ent.collision[3] = flag[3];
}


void Update::UpdatePlayer(Player& User)
{
    Collision(User);
}

void Update::UpdateFallingTile()
{
    int MapHeight = tilemap.GetMapHeight();
    int MapLength = tilemap.GetMapLength();

    for (int Y=0; Y!=MapHeight;Y++)
    {
        for (int X=0;X!=MapLength;X++)
        {
            if (tilemap.Tiles[Y][X].GetType() == 1)
            {
                if (Y < MapHeight-1 && tilemap[Y+1][X] == 0)
                {
                    std::swap(tilemap[Y][X], tilemap[Y+1][X]);
                }
                switch (rand()%2) 
                {
                case 0:
                    if (X > 0 && tilemap[Y+1][X-1] == 0)
                    {
                        std::swap(tilemap[Y][X], tilemap[Y+1][X-1]);
                    }
                    break;
                case 1:            
                    if (X < MapLength && tilemap[Y+1][X+1] == 0)
                    {
                        std::swap(tilemap[Y][X], tilemap[Y+1][X+1]);
                    }
                    break;

                default:
                    break;
                }
            }
        }
    }
}