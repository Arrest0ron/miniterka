#include "textures.h"


double perlin(int x, int y) 
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
// битовая переменная нужна для прихода к положительному значения, а double для прихода к значению от -1 до 1, хз зачем, так в алгоритмах пишут, что это стандарт. остальные числа для хаотичности
void applyPerlinNoiseInsideStones(int**& map, int MAP_LENGTH, int MAP_HEIGHT, int SEALEVEL) 
{
    double porog = 0.1; // пороговое значение для шума перлина, вероятность того, что будет сгенерировано другое число, то есть - вероятность изменения текстуры
    std::cout.precision(2);
    for (int y = 0; y < MAP_HEIGHT; ++y) 
    {
        for (int x = 0; x < MAP_LENGTH; ++x) 
        {
            // std::cout << perlinNoise(x, y) << " ";
            float diff = pow((static_cast<float>(y)/static_cast<float>(SEALEVEL))- 1 ,4);
            float noise = interpolatedNoise(x/6.f,y/6.f);
            


            if ((noise+diff>= porog) && (map[y][x] == 2) && (y > SEALEVEL))
            {
                // double noiseValue = perlin(x, y); // для шума Перлина
                // std::cout << noiseValue << " ";

                int k = rand() % 4; // тут я выебываюсь просто
                if(k == 1)
                {
                    k--;
                }
                if(k == 2)
                {
                    k++;
                }

                map[y][x] = k; // меняем на другую текстурку

            }
        }
        std::cout << "\n";
    }
}

float smoothNoise(float x, float y) {
    float corners = (perlin(x - 1, y - 1) + perlin(x + 1, y - 1) + perlin(x - 1, y + 1) + perlin(x + 1, y + 1)) / 16;
    float sides = (perlin(x - 1, y) + perlin(x + 1, y) + perlin(x, y - 1) + perlin(x, y + 1)) / 8;
    float center = perlin(x, y) / 4;
    return corners + sides + center;
}

// Функция для интерполяции шума... интерполяция - вычисление среднего значения. Т.е. выравнивание
float interpolate(float a, float b, float x) {
    float ft = x * 3.1415927;
    float f = (1 - cos(ft)) * 0.5;
    return a * (1 - f) + b * f;
}

// Функция для генерации окончательного шума Перлина
float interpolatedNoise(float x, float y) {
    int integer_X = int(x);
    float fractional_X = x - integer_X;
    int integer_Y = int(y);
    float fractional_Y = y - integer_Y;
    float v1 = smoothNoise(integer_X, integer_Y);
    float v2 = smoothNoise(integer_X + 1, integer_Y);
    float v3 = smoothNoise(integer_X, integer_Y + 1);
    float v4 = smoothNoise(integer_X + 1, integer_Y + 1);
    float i1 = interpolate(v1, v2, fractional_X);
    float i2 = interpolate(v3, v4, fractional_X);
    return interpolate(i1, i2, fractional_Y);
}
