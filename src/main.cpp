
#include <SFML/Graphics.hpp>
#include "textures.h"


const int MAP_HEIGHT = 300;
const int MAP_LENGTH = 260;
const int TILESET_SIZE = 32*32;
const int TILESET_X = 32;
const int MIN_SEALEVEL = MAP_HEIGHT/2;
const int MAX_SEALEVEL = MAP_HEIGHT/2+MAP_HEIGHT*0.05;
const int SEALEVEL = MAP_HEIGHT / 2;
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
                map[y][x] = 4;
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

// void Map::TickWater(int **& map){
//     for(int y=0; y !=MAP_HEIGHT;y++)
//     {
//         for (int x =0; x!= MAP_LENGTH; x++)
//         {
//             if (map[y][x] == 4 || map[y][x] == 5)
//             {
//                 if (y < SEALEVEL){
//                     map[y][x] = 4;
//                 }
//                 else
//                 {
//                     map[y][x] = 5;
//                 }




//                 if (y < MAP_HEIGHT-1 && map[y+1][x] == 0)
//                 {
//                     std::swap(map[y][x], map[y+1][x]);
//                 }
//                 switch (rand()%2)
//                 {
//                 case 0:
//                     if (x > 0 && map[y][x-1] == 0)
//                     {
//                         std::swap(map[y][x], map[y][x-1]);
//                     }
//                     break;
//                 case 1:            
//                     if (x < MAP_LENGTH && map[y][x+1] == 0)
//                     {
//                         std::swap(map[y][x], map[y][x+1]);
//                     }
//                     break;
//                 default:
//                     break;
//                 }
//             }
//         }
//     }
// }
// Убирает слишком маленькие зоны наполнения водой.
void WaterClean(int**& map, int MinWaterChainSize = 10)
{
    for (int x=0;x!=MAP_LENGTH;x++)
    {
 
        int CurrentHight = GetMaxHight(map,x);
        int CurrentTopTile = map[CurrentHight][x];
        
        if (CurrentTopTile !=4){continue;}
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

int main()
{
    int FREEZE = 0;
    // Установка семечка генерации как ключа для генерации всех случайных переменных.
    int GLOBAL_SEED = time(0);
    // int GLOBAL_SEED = 1710959590;
    srand(GLOBAL_SEED);

    
    std::cout << GLOBAL_SEED << "\n";

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
    

    int tileSize = 16; // Размер каждого тайла


    // Загружаем шрифт
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Ошибка загрузки шрифта
        return -1;
    }


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

    // int** tilemap = new int*[MAP_HEIGHT];
    // for (int i=0;i!=MAP_HEIGHT;i++){
    //     tilemap[i] = new int[MAP_LENGTH];
    // }

    // for (int i= 0;i!=MAP_HEIGHT;i++){
    //     for (int j=0;j!=MAP_LENGTH;j++){
    //         tilemap[i][j] = 0;
    //     }
    // }
    Map tilemap(MAP_HEIGHT, MAP_LENGTH, GLOBAL_SEED);
    Update upd(tilemap);
    try
    {
        
        // RandomWalkTopSmoothed(tilemap,SECTIONWIDTH); 
        // WaterFill(tilemap);                          
        // WaterClean(tilemap,7);
        // tilemap.PerlinCaves(Air);
        
        tilemap.RandomWalkSurface();
        tilemap.PerlinHights(Stone);
        tilemap.PerlinCaves(Diamond);
        tilemap.PerlinCaves(Redstone);
        tilemap.LiquidStripe(WaterUnder,2.9f,2.f,0.5);
        tilemap.LiquidStripe(Lava,1.2f,1.3f,0.3);

        // PerlinOre(tilemap, MAP_LENGTH, MAP_HEIGHT, SEALEVEL);
        // for (int i =0;i!=MAP_HEIGHT;i++){
        //     TickWater(tilemap);
        // }
        
    }
    catch(std::runtime_error err){
        std::cout << "\n" << err.what() << "\n";
    }
    
    sf::View NewZoom;
    if (PLAYABLE){
        // NewZoom.setSize(160,90);
    }
    NewZoom.setSize(MAP_LENGTH * tileSize, tileSize*MAP_LENGTH);
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
        float movement = 250.0f * dtAsSeconds * tileSize/2;

        // Обработка событий
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == event.KeyPressed)
            {
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    window.clear();
                    DebugNumMode =(DebugNumMode+1)%2;
                }


                if (event.key.scancode == sf::Keyboard::Scan::F){
                    FREEZE = 1;
                }
                if (event.key.scancode == sf::Keyboard::Scan::G){
                    FREEZE = 0;
                }

            }

        }

        // Нажатия клавиш

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && NewZoom.getSize().x < MAP_LENGTH*tileSize*2)
        {
            NewZoom.zoom(1.02);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && NewZoom.getSize().x > MAP_LENGTH*tileSize/10)
        {
            NewZoom.zoom(0.98);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && NewZoom.getCenter().x > 0)
        {
            NewZoom.move(-movement, 0);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)&& NewZoom.getCenter().y <MAP_HEIGHT*tileSize)
        {
            NewZoom.move(0, movement);
            window.setView(NewZoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)&& NewZoom.getCenter().x < MAP_LENGTH*tileSize)
        {

            NewZoom.move(movement, 0);
            window.setView(NewZoom);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)&& NewZoom.getCenter().y > 0)
        {
            NewZoom.move(0, -movement);
            window.setView(NewZoom);
        }
        // TickWater(tilemap);
        if (!FREEZE)
        {
            upd.tick();
        }



        // Отрисовка тайлмапа
        window.clear();
        sf::Vector2f ViewSize = NewZoom.getSize();
        sf::Vector2f ViewCenter = NewZoom.getCenter();

        float LoadedXL = (ViewCenter.x - ViewSize.x/2)/tileSize;
        LoadedXL < 0 ? LoadedXL = 0 : LoadedXL=LoadedXL;

        float LoadedXR = (ViewCenter.x + ViewSize.x/2)/tileSize;
        LoadedXR > MAP_LENGTH ? LoadedXR = MAP_LENGTH-1 :  LoadedXR=LoadedXR;

        float LoadedYU = (ViewCenter.y - ViewSize.y/2)/tileSize;
        LoadedYU < 0 ? LoadedYU = 0 : LoadedYU=LoadedYU;

        float LoadedYD = (ViewCenter.y + ViewSize.y/2)/tileSize;
        LoadedYD > MAP_HEIGHT ? LoadedYD = MAP_HEIGHT-1 :  LoadedYD=LoadedYD;

        // std::cout << "LOADING TILES FROM X = " << LoadedXL << " TO X = " << LoadedXR << "\n";
        // std::cout << "LOADING TILES FROM Y = " << LoadedYU << " TO Y = " << LoadedYD << "\n";
    
        

        for (int y = LoadedYU; y < LoadedYD; ++y)
        {  
            for (int x = LoadedXL; x < LoadedXR; ++x)
            {

                // Получаем число внутри элемента тайлмапа, обозначающее номер нужной текстурки
                int tileIndex = tilemap[y][x].GetTile();

                // Для режима с числами вместо тайлов
                if (DebugNumMode)
                {
                    // if (x%16 ==0 && y ==0)
                        {
                            sf::Text DebugNum;
                            DebugNum.setFont(font);
                            DebugNum.setCharacterSize(16);
                            DebugNum.setPosition(x * tileSize+2, y * tileSize);
                            DebugNum.setString(std::to_string(tileIndex));
                            DebugNum.setFillColor(sf::Color::White);
                            window.draw(DebugNum);
                            // 
                            continue;
                        }
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
    return 0;
}