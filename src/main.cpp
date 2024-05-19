
#include <SFML/Graphics.hpp>
#include "SFML/Window.hpp"




#include "settings.h"

#include "Handler.h"
#include "Map.h"
#include "Entity.h"
#include "Update.h"
#include "Drawing.h"
#include "Cursor.h"
#include <cmath>


int const TILESET_SIZE = 32*32;
int TILESET_X = 32;
int tileSize = 16; // Размер каждого тайла
const int MAP_HEIGHT = 320;
const int MAP_LENGTH = 320;

const int SEALEVEL = MAP_HEIGHT / 2;
const float MOVEMENTCAP = 3.0f;



const int SECTIONWIDTH = 6;
const bool PLAYABLE = true;

int DebugNumMode = 0;
int DebugTilesMode = 0;


int FREEZE = 0; // Мир не заморожен с самого начала.
int EntitiesMAX = 0;


bool IsGreaterThenLimit(sf::Vector2f& movement)
{
    if (pow(pow(movement.x,2) + pow(movement.y,2)/1.2,0.5) >= MOVEMENTCAP)
    {
        return 1;
    }
    return 0;
}

int main()
{
    Entity* Entities = new Entity[EntitiesMAX];
    EntityStack EntitiesList(EntitiesMAX,Entities);
    // Установка семечка генерации как ключа для генерации всех случайных переменных.
    int GLOBAL_SEED = time(0);
    // int GLOBAL_SEED = 1710959590;
    srand(GLOBAL_SEED);

    
    std::cout << GLOBAL_SEED << "\n";

    // Создание окна
    sf::RenderWindow MainWindow(sf::VideoMode(900, 900), "Tilemap");

    // Выбор расположения файлов для разных ос
    #ifdef WIN32
        system("chcp 65001");
        std::string player = "Player.png";
        std::string texture_4 = "textures-4.png";
        std::string arial = "Arial.ttf";
    #else
        setlocale(LC_ALL, "Russian");
        std::string player = "/home/user/Documents/GitHub/miniterka/images/players/char_01.png";
        std::string texture_4 = "/home/user/Documents/GitHub/miniterka/images/textures-4.png";
        std::string arial = "/home/user/Documents/GitHub/miniterka/images/Arial.ttf";
    #endif


    sf::Texture tileset;
    tileset.loadFromFile(texture_4);
        // Создание спрайтов для каждого тайла из тайлсета
    sf::Sprite tiles [TILESET_SIZE];

        // Установка текстур с тайлсета в тайлы
    for (int i = 0; i < TILESET_SIZE; ++i)
    {
        int x = (i % TILESET_X) * tileSize;
        int y = (i / TILESET_X) * tileSize;
        tiles[i].setTexture(tileset);
        tiles[i].setTextureRect(sf::IntRect(x, y, tileSize, tileSize));
    }

    

    


    // Загружаем шрифт
    sf::Font font;
    if (!font.loadFromFile(arial)) {
        // Ошибка загрузки шрифта
        return -1;
    }





    sf::Texture PlayerTexture;
    Player User(tileset);
    PlayerTexture.loadFromFile(player);

    User.setTexture(PlayerTexture);
    

    // Создание 
    Map tilemap(MAP_HEIGHT, MAP_LENGTH, GLOBAL_SEED);

    Update upd(tilemap,EntitiesList,EntitiesMAX,User);

    Cursor UserCursor(User,tilemap);

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


    // Создаем поле зрения
    sf::View NewZoom;
    NewZoom.setSize(MAP_LENGTH * tileSize, tileSize*MAP_LENGTH);
    NewZoom.setCenter(sf::Vector2f(MAP_LENGTH/2*tileSize,MAP_HEIGHT/2*tileSize));
    MainWindow.setView(NewZoom);

    

    if (PLAYABLE)
    {
        
        User.setPosition(sf::Vector2f(0,  tilemap.GetSurfaceHeight(MAP_LENGTH/2)*tileSize-User.GetModelHeight()-16));
        // User.setTexture(tileset);

        NewZoom.setCenter(User.GetSprite().getPosition());
        NewZoom.setSize(60*tileSize,60*tileSize);
        MainWindow.setView(NewZoom);
    }
   

    // Замер времени для гладкого перемещения
    sf::Clock clock;




    // Основной цикл окна
    while (MainWindow.isOpen())
    {

        // Получаем время прошлого цикла
        sf::Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();

        // Скорость движения относительно времени 
        float BaseSpeed = 2.0f * dtAsSeconds * tileSize/2  ;
        



        // Нажатия клавиш

        //Гравитация 
        if (User.GetCollision()[1] == 0)
            {
                User.movement.y+= BaseSpeed/1.67;
                User.movement.y = std::min(User.movement.y,MOVEMENTCAP*2);
            }

        //Движение по клавишам, с учетом предела скорости
        if (!IsGreaterThenLimit(User.movement))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && NewZoom.getSize().x < MAP_LENGTH*tileSize*2)
            {
                NewZoom.zoom(1.02);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && NewZoom.getSize().x > MAP_LENGTH*tileSize/100)
            {
                NewZoom.zoom(0.98);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
            {
                User.movement.x -= BaseSpeed;

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                User.movement.x += BaseSpeed;

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (User.GetCollision()[1] == 1))
            {
                User.movement.y-=(BaseSpeed+0.05)*30;

            }
        }

        // Остановка при крайне маленьком движении.
        if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::D))&&(!sf::Keyboard::isKeyPressed(sf::Keyboard::A))) 
        {

            User.movement.x /= (1+BaseSpeed*0.2);
            if ((-0.01 < User.movement.x) && (User.movement.x< 0.01 ))
            {
                User.movement.x = 0;
            }
        }
        if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::W))&&(!sf::Keyboard::isKeyPressed(sf::Keyboard::S))) 
        {
            User.movement.y /= (1+BaseSpeed*0.2);
            if ((-0.01 < User.movement.y) && (User.movement.y< 0.01 ))
            {
                User.movement.y = 0;
            }
        }


        //Движение с учетом коллизии
        for (int i =0 ; i != 50; i++)
        {
            upd.UpdatePlayer(User);
            std::vector<int> calledColl = User.GetCollision();
            if ((calledColl[0] == 1) && (User.movement.y<0))
            {
                User.movement.y=-0;
            }
            if ((calledColl[1] == 1) && (User.movement.y>0))
            {
                User.movement.y=0;
            }
            if ((calledColl[2] == 1) && (User.movement.x<0))
            {
                User.movement.x=0;
            }
            if ((calledColl[3] == 1) && (User.movement.x>0))
            {
                User.movement.x=-0;
            }
            if ((calledColl[0] == 2))
            {
                User.movement.y=-User.movement.y/3 -0.1;
            }
            if ((calledColl[1] == 2) )
            {
                User.movement.y=-User.movement.y/3 + 0.1;
            }
            if ((calledColl[2] == 2) )
            {
                User.movement.x=-User.movement.x/3 + 0.1;
            }
            if ((calledColl[3] == 2) )
            {
                User.movement.x=-User.movement.x/3 -0.1;
            }
            
            User.GetSprite().move(User.movement.x/50,User.movement.y/50);
        
            
            

        }
        



        // std::cout << User.movement.x << "  " << User.movement.y << " \n";
        
        

        NewZoom.setCenter(User.GetSprite().getPosition());
        MainWindow.setView(NewZoom);

        if (FREEZE)
        {
            upd.tick();
            
        }



        // Отрисовка тайлмапа
        MainWindow.clear();
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
    
        
        // Тайлы и дебаг отрисовка
        for (int y = LoadedYU; y < LoadedYD; ++y)
        {  
            for (int x = LoadedXL; x < LoadedXR; ++x)
            {

                // Получаем число внутри элемента тайлмапа, обозначающее номер нужной текстурки
                int tileIndex = tilemap.ReturnTiles()[y][x].GetTile();

                // Для режима с числами вместо тайлов
                if (DebugNumMode)
                {
                    DrawText(MainWindow,font,x * tileSize + 2,y*tileSize,tileIndex);
                    continue;
                }       

                // Отрисовка тайлов

                tiles[tileIndex].setPosition(x * tileSize, y * tileSize);
                MainWindow.draw(tiles[tileIndex]);
                if (tileIndex != 0 && DebugTilesMode)
                {
                    sf::Vertex verticesA[4] =
                    {
                        sf::Vertex(sf::Vector2f(x*tileSize,(y+1)*tileSize)),
                        sf::Vertex(sf::Vector2f((x+1)*tileSize,(y+1)*tileSize)),
                        sf::Vertex(sf::Vector2f((x+1)*tileSize,y*tileSize)),
                        sf::Vertex(sf::Vector2f(x*tileSize,y*tileSize))
                        

                    };
                    verticesA[0].color = sf::Color::Red;
                    verticesA[1].color = sf::Color::Blue;
                    verticesA[2].color = sf::Color::Green;
                    verticesA[3].color = sf::Color::Magenta;
                    
                    MainWindow.draw(verticesA, 4, sf::Quads);
                }
            }
        }

        //Отрисовка вспомогательных чисел
        DrawText(MainWindow,font,User.getGlobalBounds().left,User.getGlobalBounds().top-tileSize*5,std::to_string(User.GetCollision()[0])+ std::to_string(User.GetCollision()[1]) + std::to_string(User.GetCollision()[2])+std::to_string(User.GetCollision()[3]));
        DrawText(MainWindow,font,MAP_LENGTH/2,0,GLOBAL_SEED,24,sf::Color::Black);
        DrawText(MainWindow,font,User.getGlobalBounds().left,User.getGlobalBounds().top - tileSize*2,User.GetHealth(),16,gradientRG(100,User.GetHealth()));
        if (!(rand()%100)){User.ChangeHealth(-1);}
        // Отрисовка деталей
        

        UserCursor.UpdatePos(MainWindow);

        int TouchedY = UserCursor.BlockTouched().y;
        int TouchedX = UserCursor.BlockTouched().x;
        Tile& Touched = tilemap.ReturnTiles()[TouchedY][TouchedX];
        
        sf::Vertex verticesTouched[5] =
        {
		sf::Vertex(sf::Vector2f(TouchedX*16,TouchedY*16)),
		sf::Vertex(sf::Vector2f((TouchedX+1)*16,TouchedY*16)),
		sf::Vertex(sf::Vector2f((TouchedX+1)*16,(TouchedY+1)*16)),
		sf::Vertex(sf::Vector2f(TouchedX*16,(TouchedY+1)*16)),
        sf::Vertex(sf::Vector2f(TouchedX*16,TouchedY*16))

        };
                
        // Обработка событий
        sf::Event event;
        int LeftMouseFlag = 0;
        
        while (MainWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                MainWindow.close();
            if (event.type == event.KeyPressed)
            {
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    MainWindow.clear();
                    DebugNumMode =(DebugNumMode+1)%2;
                }


                if (event.key.scancode == sf::Keyboard::Scan::F){
                    FREEZE = 1;
                }
                if (event.key.scancode == sf::Keyboard::Scan::G){
                    FREEZE = 0;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if ((event.mouseButton.button == sf::Mouse::Left) && (LeftMouseFlag == 0) && (UserCursor.DistanceFromOwner()<10))
                {
                    Touched.SetID(0);
                    Touched.SetType(0);
                    LeftMouseFlag = 1;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased){
                if (event.mouseButton.button == sf::Mouse::Left && LeftMouseFlag == 1)
                {
                    LeftMouseFlag = 0;
                }
            }

            

        }

        MainWindow.draw(User);
        MainWindow.draw(verticesTouched,5,sf::LineStrip);
        // DrawContainingBox(MainWindow,User);
        // DrawContainingBoxInt(MainWindow,User);
        MainWindow.display();
    }
    return 0;
}
