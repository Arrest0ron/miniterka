
#include <SFML/Graphics.hpp>
#include <iostream>
#include "textures.h"
#include <ctime>
#include <cmath>

const int MAP_HEIGHT = 100;
const int MAP_LENGTH = 200;
const int TILESET_SIZE = 32*32;
const int TILESET_X = 32;
const int MIN_SEALEVEL = MAP_HEIGHT/2;
const int MAX_SEALEVEL = MAP_HEIGHT/2+MAP_HEIGHT*0.05;
const int SEALEVEL = (MIN_SEALEVEL + MAX_SEALEVEL) / 2;
const int SECTIONWIDTH = 6;

const bool PLAYABLE = true;

// Возвращает максимальный y на заданном x, на котором не пусто.
int GetMaxHight(int**& map,int x) 
{
    if (x >= MAP_LENGTH){
        throw std::runtime_error("X OVERFLOW");
    }
    if (x < 0){
        throw std::runtime_error("X UNDER 0");
    }

    for (int y=0;y!=MAP_HEIGHT;y++)
    {
        if (map[y][x] != 0)
        {
            return y;
        }
    }
    return MAP_HEIGHT-1;
}

//Наполняет водой всю пустоту ниже SEALEVEL
void WaterFill(int**& map)
{
    for (int x=0;x!=MAP_LENGTH;x++)
    {
        int maxH = GetMaxHight(map,x);
        if (maxH > SEALEVEL)
        {
            for(int y=SEALEVEL;y!=maxH;y++)
            {
                map[y][x] = 3;
            }
        }
    }
}

// Считает длину цепочки воды
int CalculateSurfaceChain(int**& map,int x){
    int WaterCounter{};
    for (int i = x;i!=MAP_LENGTH;i++)
        {
            if (map[GetMaxHight(map,i)][i] == 3)
            {
                WaterCounter ++;
            } 
            
            else{
                break;
            }
        }
    return WaterCounter;
}

// Убирает слишком маленькие зоны наполнения водой.
void WaterClean(int**& map, int MinWaterChainSize = 10)
{
    for (int x=0;x!=MAP_LENGTH;x++)
    {
 
        int CurrentHight = GetMaxHight(map,x);
        int CurrentTopTile = map[CurrentHight][x];
        
        if (CurrentTopTile !=3){continue;}
        int WaterChain = CalculateSurfaceChain(map,x);
        if (WaterChain < MinWaterChainSize)
        {

            for (int i =x;i!=x+WaterChain;i++)
            {
                for (int y = GetMaxHight(map,i);map[y][i]!=1;y++)
                {
                    if (map[y][i] == 3)
                    {
                        map[y][i] = 0;
                    }
                }

            }
            
        }
        x = std::min(x+WaterChain,MAP_LENGTH-1);
    }
}

// RandomWalk для первичной генерации линии поверхности
int**& RandomWalkTopSmoothed(int**& map, int minSectionWidth)
{

    // std::cout << seed;
    
    int randint = rand();

    //Определили начальную высоту
    int lastHeight = SEALEVEL-SEALEVEL*0.03;

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
    // Установка семечка генерации как ключа для генерации всех случайных переменных.
    int GLOBAL_SEED = time(0);
    srand(GLOBAL_SEED);

    // GLOBAL_SEED = 1710959590;
    std::cout << GLOBAL_SEED << "\n";

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


    // Загружаем шрифт
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Ошибка загрузки шрифта
        return -1;
    }

    // Отрисовка цифр вместо тайлов. (Внимание! разбивает на группы 2 на 2 блока, и рисует цифру верхнего левого из них. Не точно!)
    int DebugNumMode = 0;


    // Установка текстур с тайлсета в тайлы
    for (int i = 0; i < TILESET_SIZE; ++i)
    {
        int x = (i % TILESET_X) * tileSize;
        int y = (i / TILESET_X) * tileSize;
        tiles[i].setTexture(tileset);
        tiles[i].setTextureRect(sf::IntRect(x, y, tileSize, tileSize));
    }

    // Создание тайлмапа, заполнение пустотой

    int** tilemap = new int*[MAP_HEIGHT];
    for (int i=0;i!=MAP_HEIGHT;i++){
        tilemap[i] = new int[MAP_LENGTH];
    }

    for (int i= 0;i!=MAP_HEIGHT;i++){
        for (int j=0;j!=MAP_LENGTH;j++){
            tilemap[i][j] = 0;
        }
    }
    

    try
    {
        RandomWalkTopSmoothed(tilemap,SECTIONWIDTH); 
        WaterFill(tilemap);                          
        WaterClean(tilemap,7);
        applyPerlinNoiseInsideStones(tilemap, MAP_LENGTH, MAP_HEIGHT);
    }
    catch(std::runtime_error err){
        std::cout << "\n" << err.what() << "\n";
    }
    
    // for ( int i =0;i<MAP_LENGTH;i++){
    //     std::cout << GetMaxHight(tilemap,i) << " ";
    // }
    sf::View NewZoom;
    if (PLAYABLE){
        // NewZoom.setSize(160,90);
    }
    NewZoom.setSize(MAP_HEIGHT * tileSize, tileSize*MAP_HEIGHT);
    NewZoom.setCenter(sf::Vector2f(MAP_LENGTH/2*tileSize,MAP_HEIGHT/2*tileSize));
    window.setView(NewZoom);
   

    // Замер времени для гладкого перемещения
    sf::Clock clock;

    
    // Линия уровня воды.
    sf::VertexArray WaterLine(sf::Lines);
    sf::Vertex WaterLineStart = (sf::Vector2f(0.f,SEALEVEL*tileSize));
    sf::Vertex WaterLineEnd = (sf::Vector2f(MAP_LENGTH*tileSize,SEALEVEL*tileSize));
    WaterLineStart.color = sf::Color::Red;
    WaterLineEnd.color = sf::Color::Red;
    WaterLine.append(WaterLineStart);
    WaterLine.append(WaterLineEnd);

    // Текст с ключом генерации мира

    sf::Text SeedText;
    SeedText.setFont(font); // Устанавливаем шрифт
    SeedText.setString(std::to_string(GLOBAL_SEED)); // Устанавливаем строку текста
    SeedText.setCharacterSize(24); // Устанавливаем размер шрифта
    SeedText.setPosition(MAP_LENGTH/2,0); // Устанавливаем положение
    SeedText.setFillColor(sf::Color::Black); // Устанавливаем цвет текста




    // Основной цикл окна
    while (window.isOpen())
    {
        // Получаем время прошлого цикла
        sf::Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();

        // Скорость движения относительно времени 
        float movement = 250.0f * dtAsSeconds;

        // Обработка событий
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.key.scancode == sf::Keyboard::Scan::Escape){
                window.clear();
                DebugNumMode = 1;
            }
        }

        // Нажатия клавиш

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            NewZoom.zoom(1.05);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            NewZoom.zoom(0.95);
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

                // Получаем число внутри элемента тайлмапа, обозначающее номер нужной текстурки
                int tileIndex = tilemap[y][x];

                // Для режима с числами вместо тайлов
                if (DebugNumMode)
                {   if (x%4 ==0 && y%4 ==0)
                    {
                    sf::Text DebugNum;
                    DebugNum.setFont(font);
                    DebugNum.setCharacterSize(16);
                    DebugNum.setPosition(x * tileSize, y * tileSize);
                    DebugNum.setString(std::to_string(tileIndex));
                    DebugNum.setFillColor(sf::Color::White);
                    window.draw(DebugNum);
                    }
                    continue;
                }       

                // Отрисовка тайлов

                tiles[tileIndex].setPosition(x * tileSize, y * tileSize);
                window.draw(tiles[tileIndex]);
            }
        }
        
        


    // Вывод ключа генерации 

    window.draw(WaterLine);

    
    window.draw(SeedText);
    window.display();
    }

    delete tilemap;
    return 0;
}