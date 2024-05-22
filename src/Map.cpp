#include "Map.h"
#include "Perlin.h"
#include <iostream>





struct BlocksData
{
    unsigned ID;
    unsigned Durability;
    unsigned Type;
}; 

BlocksData BlocksINF[16]
{
    {0,0,0},
    {1,0,0},
    {2,0,0},
    {3,0,0},
    {4,0,0},
    {5,0,0},
    {6,1000,0},
    {7,3000,0},
    {8,2000,0},
    {9,0,0}

};

 std::map<std::string,BlocksData> BlocksMap {
    {"Air", BlocksINF[0]},
    // {"-", 1},
    // {"--", 2},
    // {"---", 3},
    {"WaterOver", BlocksINF[4]},
    {"WaterUnder", BlocksINF[5]},
    {"Stone", BlocksINF[6]},
    {"Diamond", BlocksINF[7]},
    {"Redstone", BlocksINF[8]},
    {"Lava", BlocksINF[9]},
    // {"----", 10},
    // {"-----", 11}
};

int Map::PerlinCaves(const std::string& OreType)
{
    float porog; // - пороговое значение для шума перлина, вероятность того, что будет сгенерировано другое число, то есть - вероятность изменения текстуры
    float Interpolation;
    float HightDiff = 1.f;
    int a =  BlocksMap[OreType].ID;
    std::cout << a;
    switch (BlocksMap[OreType].ID)
    {
    case 7:
    //"Diamond"
        porog = 0.9;
        Interpolation = 2.f;
        HightDiff = 1.5f;
        break;
    case 8:
    //"Redstone":
        porog = 0.4;
        Interpolation = 4.f;
        break;
    case 0:
    //"Air":
        porog = 0.0f;
        Interpolation = 2.5f;
        HightDiff = 2;
        break;
    case 6:
    //Stone:
        porog = 0.0f;
        Interpolation = 2.5f;
    default:
        porog = 0.f;
        Interpolation = 2.5f;
        break;
    }


    int PERLINKEY_X = rand()%100000;
    int PERLINKEY_Y = rand()%100000;
    for (int Y =0; Y < MAP_HEIGHT; ++Y) 
    {
        for (int x = 0; x < MAP_LENGTH; ++x) 
        {
            // std::cout << perlinNoise(x, Y) << " ";
            float diff = (static_cast<float>(Y)/static_cast<float>(SEA_LEVEL))*HightDiff;

            float noise = interpolatedNoise((x)/Interpolation+ PERLINKEY_X,Y/Interpolation+PERLINKEY_Y);

            if ((noise*diff> porog) && (Tiles[Y][x] == 6))
            {
                Tiles[Y][x] = BlocksMap[OreType].ID;
                
                Tiles[Y][x].SetType(2);
                // std::cout << OreType << " " << Tiles[Y][x].GetTile() << " \n";
 
            }
        }
        
    }
    return 1;
}

int Map::PerlinHights(const std::string& OreType)
{
    float porog; // - пороговое значение для шума перлина, вероятность того, что будет сгенерировано другое число, то есть - вероятность изменения текстуры
    float Interpolation;
    float HightDiff;
    porog = -0.18f;
    Interpolation = 4.f;
    srand(GLOBAL_SEED);
    double PERLINKEY_X = rand()%100;
    double PERLINKEY_Y = rand()%100;

    for (int x = 0; x < MAP_LENGTH; ++x) 
    {
        for (int Y = GetGeneratedHeight(x); Y < MAP_HEIGHT; ++Y) 
        {
            // std::cout << perlinNoise(x, Y) << " ";
            float noise = interpolatedNoise((x+PERLINKEY_X)/Interpolation,(Y+PERLINKEY_Y)/Interpolation);
            if ((noise> porog) && (Tiles[Y][x] == 0))
            {
                Tiles[Y][x] = BlocksMap[OreType].ID;
                Tiles[Y][x].SetType(2);
            }
        }
    }
    return 1;
}

int Map::LiquidStripe(const std::string& LiquidType,float UpperBoundary, float DownBoundary, float percentage)
{
    for (int Y = MAP_HEIGHT/UpperBoundary;Y!=MAP_HEIGHT/DownBoundary && Y<MAP_HEIGHT;++Y)
    {
        for (int X = 0; X != MAP_LENGTH; ++X)
        {
        
            if ((Tiles[Y][X] == 0) && (static_cast<float>(rand()%101)/100 <= percentage))
            {
                Tiles[Y][X].SetID(BlocksMap[LiquidType].ID);
                Tiles[Y][X].SetType(1);
            }
        }
    }
    return 1;
}

int Map::GetSurfaceHeight(int X)
{
    if (X >= MAP_LENGTH){
        throw std::runtime_error("X OVERFLOW");
    }
    if (X < 0){
        throw std::runtime_error("X UNDER 0");
    }

    for (int Y=0;Y!=MAP_HEIGHT;Y++)
    {
        if (Tiles[Y][X] != 0)
        {
            return Y;
        }
    }
    return MAP_HEIGHT-1;   
}

int Map::RandomWalkSurface()
{

    // std::cout << seed;
    int minSectionWidth = 5;
    
    int randint = rand();

    //Определили начальную высоту
    SEA_LEVEL = MAP_HEIGHT/2;
    int lastHeight = MAP_HEIGHT/3;

    //Это для направления движения
    int nextMove = 0;
    //Длина текущего шага
    int sectionWidth = 0;

    //проходим по всем X
    for (int x = 0; x < MAP_LENGTH; x++)
    {
        //Рандомно определяем куда идти
        nextMove = randint%3;
        randint = rand();

        //Если длина секции > макс длины секции -> меняем высоту.
        if ((nextMove == 0) && (lastHeight > 0) && (sectionWidth > minSectionWidth-rand()%3))
        {
            lastHeight--;
            sectionWidth = 0;
        }
        else if ((nextMove == 1) && (lastHeight < MAP_HEIGHT) && (sectionWidth > minSectionWidth-rand()%3))
        {
            lastHeight++;
            sectionWidth = 0;
        }
        else if ((nextMove == 3) && (lastHeight < MAP_HEIGHT) && (sectionWidth > minSectionWidth-rand()%3))
        {;
            sectionWidth = 0;
        }
        //Увеличиваем длину секции
        sectionWidth++;

        //Заполняем все под нашей высотой камнем
        int RandWidth = rand()%2;
        for (int Y = lastHeight; Y < lastHeight+5+RandWidth; Y++)
        {
            Tiles[Y][x] = 6;
            Tiles[Y][x].SetType(2);

        }

    }

    // for (int Y=0;Y!=MAP_HEIGHT;Y++){
    //     for (int X=0;X!=MAP_LENGTH;X++){
    //         std::cout << Tiles[Y][x];
    //     }
    // }
    return 1;
}

int Map::GetGeneratedHeight(int X)
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
