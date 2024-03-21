#include "textures.h"


double perlinNoise(int x, int y) 
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
// битовая переменная нужна для прихода к положительному значения, а double для прихода к значению от -1 до 1, хз зачем, так в алгоритмах пишут, что это стандарт. остальные числа для хаотичности
void applyPerlinNoiseInsideStones(int**& map, int MAP_LENGTH, int MAP_HEIGHT) 
{
    double porog = 0.65; // пороговое значение для шума перлина, вероятность того, что будет сгенерировано другое число, то есть - вероятность изменения текстуры
    for (int y = 0; y < MAP_HEIGHT; ++y) 
    {
        for (int x = 0; x < MAP_LENGTH; ++x) 
        {
            if (map[y][x] == 2) 
            {
                double noiseValue = perlinNoise(x, y); // для шума Перлина

                int k = (x * y) % 4; // тут я выебываюсь просто
                if(k == 1)
                {
                    k--;
                }
                if(k == 2)
                {
                    k++;
                }
                if (noiseValue > porog) 
                {
                    map[y][x] = k; // меняем на другую текстурку
                }
            }
        }
    }
}
