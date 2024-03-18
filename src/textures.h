#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

int** GenerateArray(int width, int height, bool empty);
void RenderMap(int** map, int width, int height, sf::RenderWindow& window, sf::Texture& texture, int tileSize);