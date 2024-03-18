
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>

const int MAP_HEIGHT = 900/4;
const int MAP_LENGTH = 900/4;
const int TILESET_SIZE = 32*32;
const int TILESET_X = 32;
const int MIN_SEALEVEL = 900/16-5;
const int MAX_SEALEVEL = 900/16+5;


int**& RandomWalkTopSmoothed(int**& map, int seed, int minSectionWidth)
{
    //Seed our random
    srand(seed);
    std::cout << seed;
    
    int randint = rand();

    //Determine the start position
    int lastHeight = MIN_SEALEVEL + randint%(MAX_SEALEVEL-MIN_SEALEVEL + 1);

    randint = rand();

    //Used to determine which direction to go
    int nextMove = 0;
    //Used to keep track of the current sections width
    int sectionWidth = 0;

    //Work through the array width
    for (int x = 0; x < MAP_LENGTH; x++)
    {
        //Determine the next move
        nextMove = randint%2;
        randint = rand();

        //Only change the height if we have used the current height more than the minimum required section width
        if (nextMove == 0 && lastHeight > 0 && sectionWidth > minSectionWidth)
        {
            lastHeight--;
            sectionWidth = 0;
        }
        else if (nextMove == 1 && lastHeight < MAP_HEIGHT && sectionWidth > minSectionWidth)
        {
            lastHeight++;
            sectionWidth = 0;
        }
        //Increment the section width
        sectionWidth++;

        //Work our way from the height down to 0
        map[lastHeight][x] = 1;
        map[lastHeight+1][x] = 1;
        for (int y = lastHeight+2; y < MAP_HEIGHT; y++)
        {
            map[y][x] = 2;
        }
    }
    // for ( int i=0;i!=MAP_HEIGHT;i++){
    //     for (int j=0;j!=MAP_LENGTH;j++){
    //         std::cout << map[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }
    //Return the modified map
    return map;
}

int main()
{
    // Создание окна


    sf::RenderWindow window(sf::VideoMode(900, 900), "Tilemap");

    // Загрузка тайлсета
    sf::Texture tileset;
    if (!tileset.loadFromFile("textures-4.png"))
    {
        std::cerr << "Failed to load tileset!" << std::endl;
        return -1;
    }

    // Создание спрайтов для каждого тайла из тайлсета
    sf::Sprite tiles[TILESET_SIZE];

    int tileSize = 4; // Размер каждого тайла

    for (int i = 0; i < TILESET_SIZE; ++i)
    {
        int x = (i % TILESET_X) * tileSize;
        int y = (i / TILESET_X) * tileSize;
        tiles[i].setTexture(tileset);
        tiles[i].setTextureRect(sf::IntRect(x, y, tileSize, tileSize));
    }

    // Тайлмап

    int** tilemap = new int*[MAP_HEIGHT];
    for (int i=0;i!=MAP_HEIGHT;i++){
        tilemap[i] = new int[MAP_LENGTH];
    }

    for (int i= 0;i!=MAP_HEIGHT;i++){
        for (int j=0;j!=MAP_LENGTH;j++){
            tilemap[i][j] = 0;
        }
    }
    srand(time(0));
    int seed = rand();
    tilemap = RandomWalkTopSmoothed(tilemap,seed,2);
    // Основной цикл приложения
    while (window.isOpen())
    {
        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Отрисовка тайлмапа
        window.clear();
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            for (int x = 0; x < MAP_LENGTH; ++x)
            {
                int tileIndex = tilemap[y][x];
                tiles[tileIndex].setPosition(x * tileSize, y * tileSize);
                window.draw(tiles[tileIndex]);
            }
        }
        window.display();
    }
    delete tilemap;
    return 0;
}