#pragma once

#include <cmath>


double perlin(int x, int Y);

float smoothNoise(float x, float Y);

// Функция для интерполяции шума... интерполяция - вычисление среднего значения. Т.е. выравнивание
float interpolate(float a, float b, float x);

// Функция для генерации окончательного шума Перлина
float interpolatedNoise(float x, float Y);