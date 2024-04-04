#include "textures.h"

double perlin(int x, int Y) 
{
    int n = x + Y * 2999;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221 + 3001) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
// битовая переменная нужна для прихода к положительному значения, а double для прихода к значению от -1 до 1, хз зачем, так в алгоритмах пишут, что это стандарт. остальные числа для хаотичности
int Map::PerlinCaves(Ore OreType)
{
    float porog; // - пороговое значение для шума перлина, вероятность того, что будет сгенерировано другое число, то есть - вероятность изменения текстуры
    float Interpolation;
    float HightDiff = 1.f;
    switch (OreType)
    {
    case Diamond:
        porog = 0.9;
        Interpolation = 2.f;
        HightDiff = 1.5f;
        break;
    case Redstone:
        porog = 0.4;
        Interpolation = 4.f;
        break;
    case Air:
        porog = 0.0f;
        Interpolation = 2.5f;
        HightDiff = 2;
        break;
    case Stone:
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
                Tiles[Y][x] = static_cast<int>(OreType);
 
            }
        }
        
    }
    return 1;
}

int Map::PerlinHights(Ore OreType)
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
                Tiles[Y][x] = static_cast<int>(OreType);
            }
        }
    }
    return 1;
}

float smoothNoise(float x, float Y) {
    float corners = (perlin(x - 1, Y - 1) + perlin(x + 1, Y - 1) + perlin(x - 1, Y + 1) + perlin(x + 1, Y + 1)) / 16;
    float sides = (perlin(x - 1, Y) + perlin(x + 1, Y) + perlin(x, Y - 1) + perlin(x, Y + 1)) / 8;
    float center = perlin(x, Y) / 4;
    return corners + sides + center;
}

int Map::LiquidStripe(Liquid LiquidType,float UpperBoundary, float DownBoundary, float percentage)
{
    for (int Y = MAP_HEIGHT/UpperBoundary;Y!=MAP_HEIGHT/DownBoundary && Y<MAP_HEIGHT;++Y)
    {
        for (int X = 0; X != MAP_LENGTH; ++X)
        {
        
            if ((Tiles[Y][X] == 0) && (static_cast<float>(rand()%101)/100 <= percentage))
            {
                Tiles[Y][X].SetID(static_cast<int>(LiquidType));
                Tiles[Y][X].SetType(1);
            }
        }
    }
    return 1;
}

int Map::GetSurfaceHeight(int X){
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

// Функция для интерполяции шума... интерполяция - вычисление среднего значения. Т.е. выравнивание
float interpolate(float a, float b, float x) {
    float ft = x * 3.1415927;
    float f = (1 - cos(ft)) * 0.5;
    return a * (1 - f) + b * f;
}

// Функция для генерации окончательного шума Перлина
float interpolatedNoise(float x, float Y) {
    int integer_X = int(x);
    float fractional_X = x - integer_X;
    int integer_Y = int(Y);
    float fractional_Y = Y - integer_Y;
    float v1 = smoothNoise(integer_X, integer_Y);
    float v2 = smoothNoise(integer_X + 1, integer_Y);
    float v3 = smoothNoise(integer_X, integer_Y + 1);
    float v4 = smoothNoise(integer_X + 1, integer_Y + 1);
    float i1 = interpolate(v1, v2, fractional_X);
    float i2 = interpolate(v3, v4, fractional_X);
    return interpolate(i1, i2, fractional_Y);
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
        nextMove = randint%2;
        randint = rand();

        //Если длина секции > макс длины секции -> меняем высоту.
        if ((nextMove == 0) && (lastHeight > 0) && (sectionWidth > minSectionWidth))
        {
            lastHeight--;
            sectionWidth = 0;
        }
        else if ((nextMove == 1) && (lastHeight < MAP_HEIGHT) && (sectionWidth > minSectionWidth))
        {
            lastHeight++;
            sectionWidth = 0;
        }
        //Увеличиваем длину секции
        sectionWidth++;

        //Заполняем все под нашей высотой камнем
        int RandWidth = rand()%2;
        for (int Y = lastHeight; Y < lastHeight+5+RandWidth; Y++)
        {
            Tiles[Y][x] = 6;

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

void Update::UpdateLiquids(){
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

void Update::UpdateEntities(){
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
        int topEntityI = (topEntity+8)/16;
        int downEntityI = (downEntity-8)/16;
        int leftEntityI = (leftEntity+8)/16;
        int rightEntityI = (rightEntity-8)/16;




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