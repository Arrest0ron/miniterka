#pragma once

#include <cmath>


double perlin(int x, int Y) 
{
    int n = x + Y * 2999;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221 + 3001) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float smoothNoise(float x, float Y) {
    float corners = (perlin(x - 1, Y - 1) + perlin(x + 1, Y - 1) + perlin(x - 1, Y + 1) + perlin(x + 1, Y + 1)) / 16;
    float sides = (perlin(x - 1, Y) + perlin(x + 1, Y) + perlin(x, Y - 1) + perlin(x, Y + 1)) / 8;
    float center = perlin(x, Y) / 4;
    return corners + sides + center;
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