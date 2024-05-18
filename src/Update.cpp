#include "Update.h"
#include "Entity.h"
#include <vector>

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
                if (Y < MapHeight-1 && tilemap.Tiles[Y+1][X] == 0)
                {
                    std::swap(tilemap.Tiles[Y][X], tilemap.Tiles[Y+1][X]);
                }
                switch (rand()%2) 
                {
                case 0:
                    if (X > 0 && tilemap.Tiles[Y][X-1] == 0)
                    {
                        std::swap(tilemap.Tiles[Y][X], tilemap.Tiles[Y][X-1]);
                    }
                    break;
                case 1:            
                    if (X < MapLength && tilemap.Tiles[Y][X+1] == 0)
                    {
                        std::swap(tilemap.Tiles[Y][X], tilemap.Tiles[Y][X+1]);
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
    // std::cout<<ent.getGlobalBounds().top << "\n";
    sf::FloatRect Bounds;
    Bounds = ent.getGlobalBounds();
    std::vector<float> BlocksAroundCoordinates  // top,down,left,right
    {
    Bounds.top-1, Bounds.top -1+ Bounds.height+2,
    Bounds.left-1,Bounds.left-1+ Bounds.width+2 
    };
    std::vector<int> BlocksAroundCoordinatesInt  // top,down,left,right
    {
    static_cast<int>(BlocksAroundCoordinates[0]/16), static_cast<int>(BlocksAroundCoordinates[1]/16),
    static_cast<int>(BlocksAroundCoordinates[2]/16), static_cast<int>(BlocksAroundCoordinates[3]/16)
    };
    std::vector<float> EntityCoordinates  // top,down,left,right
    {
    Bounds.top, Bounds.top  + Bounds.height,
    Bounds.left,Bounds.left + Bounds.width
    };
    std::vector<int> EntityCoordinatesInt  // top,down,left,right
    {
    static_cast<int>(EntityCoordinates[0]/16), static_cast<int>(EntityCoordinates[1]/16),
    static_cast<int>(EntityCoordinates[2]/16), static_cast<int>(EntityCoordinates[3]/16)
    };




    std::vector <int> flag{0,0,0,0};

    if (tilemap.Tiles[BlocksAroundCoordinatesInt[0]][EntityCoordinatesInt[2]].GetType() == 2)
    {
        flag[0] = 1;
    }
    if (tilemap.Tiles[BlocksAroundCoordinatesInt[0]][EntityCoordinatesInt[3]].GetType() == 2)
    {
        flag[0] = 1;
    }

    if (tilemap.Tiles[BlocksAroundCoordinatesInt[1]][EntityCoordinatesInt[2]].GetType() == 2)
    {
        flag[1] = 1;
    }
    if (tilemap.Tiles[BlocksAroundCoordinatesInt[1]][EntityCoordinatesInt[3]].GetType() == 2)
    {
        flag[1] = 1;
    }

    if (tilemap.Tiles[EntityCoordinatesInt[0]][BlocksAroundCoordinatesInt[2]].GetType() == 2)
    {
        flag[2] = 1;
    }
    if (tilemap.Tiles[EntityCoordinatesInt[1]][BlocksAroundCoordinatesInt[2]].GetType() == 2)
    {
        flag[2] = 1;
    }

    if (tilemap.Tiles[EntityCoordinatesInt[0]][BlocksAroundCoordinatesInt[3]].GetType() == 2)
    {
        flag[3] = 1;
    }
    if (tilemap.Tiles[EntityCoordinatesInt[1]][BlocksAroundCoordinatesInt[3]].GetType() == 2)
    {
        flag[3] = 1;
    }

    


    if (tilemap.Tiles[EntityCoordinatesInt[0]][EntityCoordinatesInt[2]].GetType() == 2)
    {
        flag[0] = 2;
        flag[2] = 2;
    }
    if (tilemap.Tiles[EntityCoordinatesInt[0]][EntityCoordinatesInt[3]].GetType() == 2)
    {
        flag[0] = 2;
        flag[3] = 2;
    }
    if (tilemap.Tiles[EntityCoordinatesInt[1]][EntityCoordinatesInt[2]].GetType() == 2)
    {
        flag[1] = 2;
        flag[2] = 2;
    }
    if (tilemap.Tiles[EntityCoordinatesInt[1]][EntityCoordinatesInt[3]].GetType() == 2)
    {
        flag[1] = 2;
        flag[3] = 2;
    }

    ent.collision[0] = flag[0];
    ent.collision[1] = flag[1];
    ent.collision[2] = flag[2];
    ent.collision[3] = flag[3];

    return;


}

// void Update::TouchLava(Entity& ent)
// {
    
// }

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
                if (Y < MapHeight-1 && tilemap.Tiles[Y+1][X] == 0)
                {
                    std::swap(tilemap.Tiles[Y][X], tilemap.Tiles[Y+1][X]);
                }
                switch (rand()%2) 
                {
                case 0:
                    if (X > 0 && tilemap.Tiles[Y+1][X-1] == 0)
                    {
                        std::swap(tilemap.Tiles[Y][X], tilemap.Tiles[Y+1][X-1]);
                    }
                    break;
                case 1:            
                    if (X < MapLength && tilemap.Tiles[Y+1][X+1] == 0)
                    {
                        std::swap(tilemap.Tiles[Y][X], tilemap.Tiles[Y+1][X+1]);
                    }
                    break;

                default:
                    break;
                }
            }
        }
    }
}