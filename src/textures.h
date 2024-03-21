#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <iostream>
#include <cmath>

void applyPerlinNoiseInsideStones(int**& map, int MAP_LENGTH, int MAP_HEIGHT, int SEALEVEL);
double perlin(int x, int y);

float smoothNoise(float x, float y);
float interpolate(float a, float b, float x);
float interpolatedNoise(float x, float y);