
#include <SFML/Graphics.hpp>
#include "SFML/Window.hpp"





#include "Handler.h"
#include "Map.h"
#include "Entity.h"
#include "Update.h"
#include "Drawing.h"
#include "Cursor.h"
#include <cmath>

#include "settings.h"



// bool IsGreaterThenLimitX(sf::Vector2f& movement)
// {
//     if (pow(pow(movement.x,2) + pow(movement.y,2)/1.4,0.5) >= MOVEMENTCAP)
//     {
//         return 1;
//     }
//     return 0;
// }
bool IsGreaterThenLimitX(sf::Vector2f& movement)
{
    if (pow(movement.x,2) >= MOVEMENTCAP)
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
        std::string player = "char_01.png";
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
        tilemap.PerlinHights("Stone");
        tilemap.PerlinCaves("Diamond");
        tilemap.PerlinCaves("Redstone");
        tilemap.LiquidStripe("WaterUnder",2.9f,2.f,0.5);
        tilemap.LiquidStripe("Lava",1.2f,1.3f,0.3);
        tilemap.Walls();
        
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
        float userY = tilemap.GetSurfaceHeight(MAP_LENGTH/2)*tileSize-User.GetModelHeight();
        float userX = MAP_LENGTH/2*tileSize;
        User.setPosition(sf::Vector2f(userX,  userY));
        // User.setTexture(tileset);

        NewZoom.setCenter(User.GetSprite().getPosition());
        NewZoom.setSize(User.GetModelLength()*60*tileSize,60*User.GetModelHeight()*tileSize);
        MainWindow.setView(NewZoom);
    }
   

    // Замер времени для гладкого перемещения
    sf::Clock clock;
    sf::Clock TimeFLB;

    float TotalTime = 0;




    // Основной цикл окна
    while (MainWindow.isOpen())
    {

        // Получаем время прошлого цикла
        sf::Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();
        
        TotalTime += dtAsSeconds;

        // Скорость движения относительно времени 
        float BaseSpeed = 2.0f * dtAsSeconds * tileSize/2  ;
        



        // Нажатия клавиш

        //Гравитация 
        if (User.GetCollision()[1] == 0)
        {
            User.movement.y+= BaseSpeed/1.67;
            User.movement.y = std::min(User.movement.y,MOVEMENTCAP*2);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && NewZoom.getSize().x < MAP_LENGTH*tileSize*2)
        {
            NewZoom.zoom(1.02);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && NewZoom.getSize().x > MAP_LENGTH*tileSize/100)
        {
            NewZoom.zoom(0.98);
        }
        //Движение по клавишам, с учетом предела скорости


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && ( (!IsGreaterThenLimitX(User.movement)) || User.movement.x > 0 ))
        {
            User.movement.x -= BaseSpeed/3 + BaseSpeed/3*2*(abs(User.movement.x)/MOVEMENTCAP);

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)&& ( (!IsGreaterThenLimitX(User.movement)) || User.movement.x < 0 ))
        {
            User.movement.x += BaseSpeed/3 + BaseSpeed/3*2*(abs(User.movement.x)/MOVEMENTCAP);

        }

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (User.GetCollision()[1] == 1))
        {
            User.movement.y-=(BaseSpeed+0.05)*30;

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
        
        UserCursor.UpdatePos(MainWindow);

        int TouchedY = UserCursor.BlockTouched().y;
        int TouchedX = UserCursor.BlockTouched().x;
                    
            
        sf::Vertex verticesTouched[5] =
        {
        sf::Vertex(sf::Vector2f(TouchedX*tileSize,TouchedY*tileSize)),
        sf::Vertex(sf::Vector2f((TouchedX+1)*tileSize,TouchedY*tileSize)),
        sf::Vertex(sf::Vector2f((TouchedX+1)*tileSize,(TouchedY+1)*tileSize)),
        sf::Vertex(sf::Vector2f(TouchedX*tileSize,(TouchedY+1)*tileSize)),
        sf::Vertex(sf::Vector2f(TouchedX*tileSize,TouchedY*tileSize))

        };

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (TimeFLB.getElapsedTime().asSeconds() > 0.3) )
        {
            verticesTouched[0].color = sf::Color::Red;
            verticesTouched[1].color = sf::Color::Blue;
            verticesTouched[2].color = sf::Color::Green;
            verticesTouched[3].color = sf::Color::Magenta;
            verticesTouched[4].color = sf::Color::Red;
            if (( TouchedX <1) || (TouchedX>=(MAP_LENGTH-1)))
            {
                TimeFLB.restart();

            }
            else if (( TouchedY <1) || (TouchedY>=(MAP_HEIGHT-1)))
            {
                TimeFLB.restart();

            }
            else
            {
                Tile& Touched = tilemap.ReturnTiles()[TouchedY][TouchedX];
                if ((Touched.GetTile() != 0) && (TimeFLB.getElapsedTime().asSeconds() > 0.3) && (UserCursor.DistanceFromOwner()<6 * tileSize) && (Touched.GetType()!=1) )
                {


                if (Touched.GetDurability() <= 0)
                {
                    Touched.SetBlock(BlocksMap["Air"]);
                    TimeFLB.restart();
                }
            }


                // else 
                // {
                //     TimeFLB.restart();
                //     // Touched.ReduceDurability(5);
                //     std::cout << Touched.GetDurability();
                // }
                
            }
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

        int LoadedXL = (ViewCenter.x - ViewSize.x/2)/tileSize;
        LoadedXL < 0 ? LoadedXL = 0 : LoadedXL=LoadedXL;

        int LoadedXR = (ViewCenter.x + ViewSize.x/2)/tileSize + 1;
        LoadedXR > MAP_LENGTH ? LoadedXR = MAP_LENGTH :  LoadedXR=LoadedXR;

        int LoadedYU = (ViewCenter.y - ViewSize.y/2)/tileSize;
        LoadedYU < 0 ? LoadedYU = 0 : LoadedYU=LoadedYU;

        int LoadedYD = (ViewCenter.y + ViewSize.y/2)/tileSize + 1;
        LoadedYD > MAP_HEIGHT ? LoadedYD = MAP_HEIGHT :  LoadedYD=LoadedYD;

        std::vector<int> Loaded{LoadedYU,LoadedYD,LoadedXL,LoadedXR};
        // std::cout << "LOADING TILES FROM X = " << LoadedXL << " TO X = " << LoadedXR << "\n";
        // std::cout << "LOADING TILES FROM Y = " << LoadedYU << " TO Y = " << LoadedYD << "\n";
    
        
        // Тайлы и дебаг отрисовка
        sf::VertexArray MAP(sf::PrimitiveType::Quads) ;
        MAP.resize(MAP_LENGTH*MAP_HEIGHT*4);

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
                if ( DebugTilesMode && tileIndex != 0)
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
        DrawText(MainWindow,font,User.getGlobalBounds().left,User.getGlobalBounds().top - tileSize*2,User.GetHealth(),tileSize,gradientRG(100,User.GetHealth()));
        
        
        DrawText(MainWindow,font,NewZoom.getCenter().x-NewZoom.getSize().x/2,NewZoom.getCenter().y-NewZoom.getSize().y/2,TotalTime,tileSize);

        if (!(rand()%100)){User.ChangeHealth(-1);}
        // Отрисовка деталей
        


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
            // if (event.type == sf::Event::MouseButtonPressed){
            //     if ((event.mouseButton.button == sf::Mouse::Left) && (LeftMouseFlag == 0) )
            //     {

            //     }
            // }
            // if (event.type == sf::Event::MouseButtonReleased){
            //     if (event.mouseButton.button == sf::Mouse::Left && LeftMouseFlag == 1)
            //     {
            //         LeftMouseFlag = 0;
            //     }
            // }
        }

        MainWindow.draw(User); 
        MainWindow.draw(verticesTouched,5,sf::LineStrip); // Оверлей выбранного тайла
        // DrawContainingBox(MainWindow,User);
        // DrawContainingBoxInt(MainWindow,User);
        MainWindow.display();
    }
    return 0;
}
