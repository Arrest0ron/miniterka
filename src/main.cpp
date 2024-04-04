
#include <SFML/Graphics.hpp>
#include "textures.h"


const int MAP_HEIGHT = 300;
const int MAP_LENGTH = 260;
const int TILESET_SIZE = 32*32;
const int TILESET_X = 32;
const int SEALEVEL = MAP_HEIGHT / 2;
const int SECTIONWIDTH = 6;

const bool PLAYABLE = true;

// Убирает слишком маленькие зоны наполнения водой.
// void WaterClean(int**& map, int MinWaterChainSize = 10)
// {
//     for (int x=0;x!=MAP_LENGTH;x++)
//     {
 
//         int CurrentHight = GetMaxHight(map,x);
//         int CurrentTopTile = map[CurrentHight][x];
        
//         if (CurrentTopTile !=4){continue;}
//         int WaterChain = CalculateSurfaceChain(map,x);
//         if (WaterChain < MinWaterChainSize)
//         {

//             for (int i =x;i!=x+WaterChain;i++)
//             {
//                 for (int y = GetMaxHight(map,i);map[y][i]!=1;y++)
//                 {
//                     if (map[y][i] == 3)
//                     {
//                         map[y][i] = 0;
//                     }
                    
//                 }

//             }
            
//         }
//         x = std::min(x+WaterChain,MAP_LENGTH-1);
//     }
// }

void MovementCap(sf::Vector2f& movement)
{
    float MOVEMENTCAP = 4.0f;
    if ((movement.x) && (movement.y))
    {
        MOVEMENTCAP/=1.42;
    }
    // MOVEMENTCAP = (std::pow(movement.x,2)+std::pow(movement.y,2))
    if (movement.x > MOVEMENTCAP)
    {
        movement.x = MOVEMENTCAP;
    }
    if (movement.y > MOVEMENTCAP)
    {
        movement.y = MOVEMENTCAP;
    }
    if (movement.x < -MOVEMENTCAP)
    {
        movement.x = -MOVEMENTCAP;
    }
    if (movement.y < -MOVEMENTCAP)
    {
        movement.y = -MOVEMENTCAP;
    }

}


int main()
{
    int FREEZE = 0;
    int EntitiesMAX = 0;
    Entity* Entities = new Entity[EntitiesMAX];
    EntityStack EntitiesList(EntitiesMAX,Entities);
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
    Player User;
    Map tilemap(MAP_HEIGHT, MAP_LENGTH, GLOBAL_SEED);
    Update upd(tilemap,EntitiesList,EntitiesMAX,User);
    try
    {
        
        tilemap.RandomWalkSurface();
        tilemap.PerlinHights(Stone);
        tilemap.PerlinCaves(Diamond);
        tilemap.PerlinCaves(Redstone);
        tilemap.LiquidStripe(WaterUnder,2.9f,2.f,0.5);
        tilemap.LiquidStripe(Lava,1.2f,1.3f,0.3);
        
    }

    catch(std::runtime_error err){
        std::cout << "\n" << err.what() << "\n";
    }
    
    sf::View NewZoom;
    NewZoom.setSize(MAP_LENGTH * tileSize, tileSize*MAP_LENGTH);
    NewZoom.setCenter(sf::Vector2f(MAP_LENGTH/2*tileSize,MAP_HEIGHT/2*tileSize));
    window.setView(NewZoom);

    

    if (PLAYABLE)
    {
        
        User.setPosition(sf::Vector2f(MAP_LENGTH/2*tileSize,  tilemap.GetSurfaceHeight(MAP_LENGTH/2)*tileSize-User.GetModelHeight()-16));
        User.setTexture(tileset);

        NewZoom.setCenter(User.GetSprite().getPosition());
        NewZoom.setSize(60*tileSize,60*tileSize);
        window.setView(NewZoom);
    }
   

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
        float BaseSpeed = 2.5f * dtAsSeconds * tileSize/2;
        

        int LeftMouseFlag = 0;
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
            // if (event.type == sf::Event::MouseButtonPressed){
            //     if ((event.mouseButton.button == sf::Mouse::Left) && (LeftMouseFlag == 0))
            //     {
            //         sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            //         sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            //         User.GetSprite().setPosition(worldPos);
            //         LeftMouseFlag = 1;
            //     }
            // }
            // if (event.type == sf::Event::MouseButtonReleased){
            //     if (event.mouseButton.button == sf::Mouse::Left && LeftMouseFlag == 1)
            //     {
            //         LeftMouseFlag = 0;
            //     }
            // }

            

        }

        // Нажатия клавиш

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && NewZoom.getSize().x < MAP_LENGTH*tileSize*2)
        {
            NewZoom.zoom(1.02);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && NewZoom.getSize().x > MAP_LENGTH*tileSize/10)
        {
            NewZoom.zoom(0.98);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && NewZoom.getCenter().x > 0)
        {
            User.movement.x -= BaseSpeed;

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)&& NewZoom.getCenter().x < MAP_LENGTH*tileSize)
        {
            User.movement.x += BaseSpeed;

        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)&& NewZoom.getCenter().y <MAP_HEIGHT*tileSize)
        {   
            User.movement.y+=BaseSpeed;

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)&& NewZoom.getCenter().y > 0)
        {
            User.movement.y-=BaseSpeed;

        }
        if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::D))&&(!sf::Keyboard::isKeyPressed(sf::Keyboard::A))) 
        {

            User.movement.x /= 1+BaseSpeed*0.2;
            if ((-0.01 < User.movement.x) && (User.movement.x< 0.01 ))
            {
                User.movement.x = 0;
            }
        }
        if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::W))&&(!sf::Keyboard::isKeyPressed(sf::Keyboard::S))) 
        {
            User.movement.y /= 1+BaseSpeed*0.2;
            if ((-0.01 < User.movement.y) && (User.movement.y< 0.01 ))
            {
                User.movement.y = 0;
            }
        }




        MovementCap(User.movement);





        std::cout << User.GetSprite().getGlobalBounds().left << " " << User.movement.x << "\n";

        if ((User.GetCollision()[0] == 1) && (User.movement.y>0))
        {
            User.movement.y=0;
        }
        if ((User.GetCollision()[1] == 1) && (User.movement.y<0))
        {
            User.movement.y=0;
        }
        if ((User.GetCollision()[2] == 1) && (User.movement.x<0))
        {
            User.movement.x=0;
        }
        if ((User.GetCollision()[3] == 1) && (User.movement.x>0))
        {
            User.movement.x=0;
        }
        // else(moe)
        
        // std::cout << User.getGlobalBounds().top;
        User.GetSprite().move(User.movement);
        NewZoom.setCenter(User.GetSprite().getPosition());
        window.setView(NewZoom);

        if (!FREEZE)
        {
            upd.tick();
            upd.UpdatePlayer(User);
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
        sf::Text DebugNumUser;
        DebugNumUser.setFont(font);
        DebugNumUser.setCharacterSize(16);
        DebugNumUser.setPosition(User.getGlobalBounds().left,User.getGlobalBounds().top-tileSize*2);
        DebugNumUser.setString(std::to_string(User.GetCollision()[0])+ std::to_string(User.GetCollision()[1]) + std::to_string(User.GetCollision()[2])+std::to_string(User.GetCollision()[3]));
        DebugNumUser.setFillColor(sf::Color::Black);
        window.draw(DebugNumUser);

        


        // Вывод ключа генерации 
        window.draw(User);
        window.draw(WaterLine);

        
        window.draw(SeedText);
        window.display();
    }
    return 0;
}