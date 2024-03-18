
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>

const int MAP_HEIGHT = 250;
const int MAP_LENGTH = 500;
const int TILESET_SIZE = 32*32;
const int TILESET_X = 32;
const int MIN_SEALEVEL = MAP_HEIGHT/2;
const int MAX_SEALEVEL = MAP_HEIGHT/2-MAP_HEIGHT*0.05;
const bool PLAYABLE = true;

int GetMaxHight(int**& map,int x)
{
    for (int y=0;y!=MAP_HEIGHT;y++)
    {
        if (map[y][x] != 0)
        {
            return y;
        }
    }
}
int**& WaterFill(int**& map)
{
    for (int x=0;x!=MAP_LENGTH;x++)
    {
        int maxH = GetMaxHight(map,x);
        if (maxH > MIN_SEALEVEL)
        {
            for(int y=MIN_SEALEVEL;y!=maxH;y++)
            {
                map[y][x] = 3;
            }
        }
        
    }
    return map;

}

// bool CheckForPoolSize(int**&map, int x, int y){
//     if (x > MAP_LENGTH-4){
//         max
//     }
// }
int**& RandomWalkTopSmoothed(int**& map, int seed, int minSectionWidth)
{
    // Установили генерацию относительно семечка 
    srand(seed);
    // std::cout << seed;
    
    int randint = rand();

    //Определили начальную высоту
    int lastHeight = MIN_SEALEVEL - randint%(-MAX_SEALEVEL+MIN_SEALEVEL);

    randint = rand();

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
        else if ((nextMove == 1) && (lastHeight < MAP_HEIGHT) && (sectionWidth > minSectionWidth) )
        {
            lastHeight++;
            sectionWidth = 0;
        }
        //Увеличиваем длину секции
        sectionWidth++;

        //Заполняем все под нашей высотой землей в два слоя и камнем ниже.
        std::cout << lastHeight << " ";
        map[lastHeight][x] = 1;
        if (lastHeight + 1 < MAP_HEIGHT){
            map[lastHeight+1][x] = 1;
        }
        for (int y = lastHeight+2; y < MAP_HEIGHT; y++)
        {
            map[y][x] = 2;
        }
    }

    return map;
}

int main()
{
    // Создание окна


    sf::RenderWindow window(sf::VideoMode(600, 600), "Tilemap");

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
    tilemap = WaterFill(tilemap);
    // for ( int i =0;i<MAP_LENGTH;i++){
    //     std::cout << GetMaxHight(tilemap,i) << " ";
    // }
    sf::View NewZoom;
    if (PLAYABLE){
        NewZoom.setSize(160,90);
    }
    NewZoom.setSize(MAP_HEIGHT * tileSize, tileSize*MAP_HEIGHT);
    NewZoom.setCenter(sf::Vector2f(MAP_LENGTH/2*tileSize,MAP_HEIGHT/2*tileSize));
    window.setView(NewZoom);
   
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();
        // Обработка событий
        sf::Event event;
        float movement = 250.0f * dtAsSeconds;


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            NewZoom.zoom(1.2);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            NewZoom.zoom(0.8);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            NewZoom.move(-movement, 0);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            NewZoom.move(0, movement);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            NewZoom.move(movement, 0);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            NewZoom.move(0, -movement);
            window.setView(NewZoom);
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
        sf::VertexArray WaterLine(sf::Lines);
        sf::Vertex WaterLineStart = (sf::Vector2f(0.f,MIN_SEALEVEL*tileSize));
        sf::Vertex WaterLineEnd = (sf::Vector2f(MAP_LENGTH*tileSize,MIN_SEALEVEL*tileSize));
        WaterLineStart.color = sf::Color::Red;
        WaterLineEnd.color = sf::Color::Red;
        WaterLine.append(WaterLineStart);
        WaterLine.append(WaterLineEnd);
        window.draw(WaterLine);


        window.display();
    }\
    delete tilemap;
    return 0;
}