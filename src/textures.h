#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

void applyPerlinNoiseInsideStones(int**& map, int MAP_LENGTH, int MAP_HEIGHT);
double perlinNoise(int x, int y);