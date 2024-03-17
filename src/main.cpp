#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

const int MAP_WIDTH = 128;
const int MAP_HEIGHT = 128;
const int TILE_SIZE = 16;

enum TileType {
    Air,
    Cloud,
    Grass,
    Dirt,
    Water
};

// Функция для вычисления шума Перлина в заданных координатах
float perlin(float x, float y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

// Функция для сглаживания шума Перлина
float smoothNoise(float x, float y) {
    float corners = (perlin(x - 1, y - 1) + perlin(x + 1, y - 1) + perlin(x - 1, y + 1) + perlin(x + 1, y + 1)) / 16;
    float sides = (perlin(x - 1, y) + perlin(x + 1, y) + perlin(x, y - 1) + perlin(x, y + 1)) / 8;
    float center = perlin(x, y) / 4;
    return corners + sides + center;
}

// Функция для интерполяции
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

int main()
{
    // Создаем окно размером 128*16 x 128*16 пикселей
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE), "Tilemap");

    // Создаем двумерный массив для хранения типов тайлов
    TileType tileMap[MAP_WIDTH][MAP_HEIGHT];

    // Генерируем карту с использованием шума Перлина
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            float noise = interpolatedNoise(x / 16.0f, y / 16.0f); // Масштабируем координаты для более гладкого шума
            if (noise > 0.5) {
                tileMap[x][y] = Dirt;
            } else if (noise > 0.4) {
                tileMap[x][y] = Grass;
            } else if (noise > 0.3) {
                tileMap[x][y] = Water;
            } else if (noise > 0.2) {
                tileMap[x][y] = Cloud;
            } else {
                tileMap[x][y] = Air;
            }
        }
    }

    // Главный цикл программы
    while (window.isOpen())
    {
        // Проверка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Отрисовка тайлов
        window.clear(sf::Color::Blue); // Фон - голубой цвет
        for (int x = 0; x < MAP_WIDTH; ++x) {
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                switch (tileMap[x][y]) {
                    case Grass:
                        tile.setFillColor(sf::Color::Green);
                        break;
                    case Dirt:
                        tile.setFillColor(sf::Color(139, 69, 19)); // Коричневый цвет для земли
                        break;
                    case Water:
                        tile.setFillColor(sf::Color::Blue);
                        break;
                    case Cloud:
                        tile.setFillColor(sf::Color::White);
                        break;
                    default:
                        continue;
                }
                window.draw(tile);
            }
        }
        window.display();
    }

    return 0;
}