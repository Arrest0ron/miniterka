
#include <SFML/Graphics.hpp>
#include "SFML/Window.hpp"





#include "Handler.h"
#include "Map.h"
#include "Entity.h"
#include "Update.h"
#include "Drawing.h"
#include "Cursor.h"
#include "Dynamics.h"
#include "Menu.h"
#include "AnimationManager.hpp"
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

int main()
{
    
    int num = menu();
    // Установка семечка генерации как ключа для генерации всех случайных переменных.
    int GLOBAL_SEED = time(0);
    // int GLOBAL_SEED = 1710959590;
    srand(GLOBAL_SEED);

    
    std::cout << GLOBAL_SEED << "\n";

    // Создание окна
    sf::RenderWindow MainWindow(sf::VideoMode(900, 900), "Tilemap");
    MainWindow.setFramerateLimit(60);
        
    sf::Texture animationSpritesheet;
    
    
    std::string player;
    // Выбор расположения файлов для разных ос
    #ifdef WIN32
        system("chcp 65001");
        player = "char_01.png";
        if(num == 1){
            player = "char_01.png";
        }
        else{
            player = "char_02.png";
        }
        std::string texture_4 = "textures-4.png";
        std::string arial = "Arial.ttf";
        animationSpritesheet.loadFromFile("CatWalk.png");
    #else
        setlocale(LC_ALL, "Russian");
        if(num == 1)
        {
            player = "/home/user/Documents/GitHub/miniterka/images/players/char_01.png";
        }
        else
        {
            player = "/home/user/Documents/GitHub/miniterka/images/players/char_02.png";
        }
        
        std::string texture_4 = "/home/user/Documents/GitHub/miniterka/images/textures-4.png";
        std::string arial = "/home/user/Documents/GitHub/miniterka/images/Arial.ttf";
        animationSpritesheet.loadFromFile("/home/user/Documents/GitHub/miniterka/images/Entities/CatWalk.png");
    #endif

    //Загружаем текстуры
    sf::Texture tileset;
    if (!tileset.loadFromFile(texture_4)) 
    {
        // Ошибка загрузки текстур
        return -1;
    }

    // Загружаем шрифт
    sf::Font font;
    if (!font.loadFromFile(arial)) {
        // Ошибка загрузки шрифта
        return -1;
    }


    // Игрок 

    sf::Texture PlayerTexture;
    PlayerTexture.setRepeated(true);
    PlayerTexture.loadFromFile(player);
    PlayerTexture.setRepeated(true);

    Player User;
    User.setTexture(PlayerTexture);
    User.GetSprite().setTextureRect(sf::IntRect(0,0,User.GetModelLength(),User.GetModelHeight()));


    // Set the number of frames in the sheet
    sf::Vector2i frameRectangle(8,0); // A total of 16 frames

    // Set the size of each frame of our animation
    sf::Vector2i spriteSize(24, 17);

    // Create our animation manager instance
    AnimationManager am;
    am.addAnimation("Walking", animationSpritesheet, frameRectangle, spriteSize,sf::Vector2i(0, 0),0);
    // am.setAnimationFrequency("Walking", 30023042);
    EntityStack EntitiesList;
    Entity Cat("cat",24,17);
    // Cat.setTexture(animationSpritesheet);
    // Cat.setPosition(sf::Vector2f(32,32));
    EntitiesList.CreateEntity(24,17,"Cat");
    


   







    

    // Карта, обработчик и курсор пользователя

    Map tilemap(MAP_HEIGHT, MAP_LENGTH, GLOBAL_SEED);

    Update upd(tilemap,EntitiesList,EntitiesMAX,User);

    Cursor UserCursor(User,tilemap);

    // Генерируем карту

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

    // Информация об ошибке

    catch(std::runtime_error err){
        std::cout << "\n" << err.what() << "\n";
    }


    // Создаем поле зрения пользователя
    sf::View NewZoom;


    float userY = tilemap.GetSurfaceHeight(MAP_LENGTH/2)*tileSize-User.GetModelHeight();
    float userX = MAP_LENGTH/2*tileSize;
    User.setPosition(sf::Vector2f(userX,  userY));
    EntitiesList[0].setPosition(sf::Vector2f((MAP_LENGTH/2-1)*tileSize, tilemap.GetSurfaceHeight(MAP_LENGTH/2-1)*tileSize-2*EntitiesList[0].GetModelHeight()));



    NewZoom.setCenter(User.GetSprite().getPosition());
    NewZoom.setSize(User.GetModelLength()*tileSize,User.GetModelHeight()*tileSize);
    MainWindow.setView(NewZoom);

   

    // Замер времени для гладкого перемещения и поломки блоков
    sf::Clock clock;
    sf::Clock TimeFLB;


    // Суммарное время открытого окна
    float TotalTime = 0;


    // Массив для отрисовки примитивов - тайлов квадратной формы
    sf::VertexArray MAP(sf::PrimitiveType::Quads) ;
    

    // Состояние для отрисовки текстур примитивов
    sf::RenderStates states;
    states.texture = &tileset;

    double D_FPS = 0;
    int FPS;
    int Frames = 0;

    // Основной цикл окна
    while (MainWindow.isOpen())
    {
        
        // Получаем время прошлого цикла
        sf::Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();
        
        TotalTime += dtAsSeconds;

        // Скорость движения относительно времени 
        float BaseSpeed = 2.0f * dtAsSeconds * tileSize  ;
        


        ViewDynamics(NewZoom);
        // Нажатия клавиш

        //Гравитация 
        Gravitation(BaseSpeed,User);
        Gravitation(BaseSpeed,EntitiesList[0]);

        FollowingLogic(BaseSpeed,EntitiesList[0],User);


        //Движение по клавишам, с учетом предела скорости


        MovementCalculation(BaseSpeed,upd,User);
        CollisionDynamic(BaseSpeed,upd,User);

        CollisionDynamic(BaseSpeed,upd,EntitiesList[0]);


        
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
                    if (Touched.GetTile()== BlocksMap["Diamond"].ID)
                    {
                        User.IncreaseScore(50);
                    }
                    if (Touched.GetTile()== BlocksMap["Redstone"].ID)
                    {
                        User.IncreaseScore(10);
                    }
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
    
        if (FREEZE)
        {
            upd.tick(Loaded,User);
            
        }

        // Тайлы и дебаг отрисовка

        Tile** TileArray = tilemap.ReturnTiles();

        if (User.GetHealth()<=0)
        {
            float userY = tilemap.GetSurfaceHeight(MAP_LENGTH/2)*tileSize-User.GetModelHeight();
            float userX = MAP_LENGTH/2*tileSize;
            User.setPosition(sf::Vector2f(userX,  userY));
            User.SetHealth(100);
            User.SetScore(100);
        }
        

        MAP.resize((LoadedYU - LoadedYD)*(LoadedXL-LoadedXR)*4);
        for (int y = LoadedYU; y < LoadedYD; ++y)
        {  
            for (int x = LoadedXL; x < LoadedXR; ++x)
            {

                sf::Vertex* quad = &MAP[((x-LoadedXL) + (y-LoadedYU) * (LoadedXR-LoadedXL)) * 4];

                int tu = TileArray[y][x].GetTile();
                quad[0].position = sf::Vector2f(x * tileSize, y * tileSize);
                quad[1].position = sf::Vector2f((x + 1) * tileSize, y * tileSize);
                quad[2].position = sf::Vector2f((x + 1) * tileSize, (y + 1) * tileSize);
                quad[3].position = sf::Vector2f(x * tileSize, (y + 1) * tileSize);

                quad[0].texCoords = sf::Vector2f(tu * tileSize, 0);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize, 0);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize, tileSize);
                quad[3].texCoords = sf::Vector2f(tu * tileSize, tileSize);
                
            }
        }
        
        MainWindow.draw(MAP,states);

        // Отрисовка кадра зафиксирована и учтена в FPS
        D_FPS += dtAsSeconds;
        Frames +=1;
        if (Frames == 60)
        {
            FPS = round(Frames/D_FPS);
            Frames=0;
            D_FPS = 0;
        }




        //Отрисовка вспомогательных чисел
        // DrawText(MainWindow,font,User.getGlobalBounds().left,User.getGlobalBounds().top-tileSize*5,std::to_string(User.GetCollision()[0])+ std::to_string(User.GetCollision()[1]) + std::to_string(User.GetCollision()[2])+std::to_string(User.GetCollision()[3]));
        DrawText(MainWindow,font,MAP_LENGTH/2,0,GLOBAL_SEED,24,sf::Color::Black);
        DrawText(MainWindow,font,User.getGlobalBounds().left-tileSize*0.5,User.getGlobalBounds().top - tileSize*2,User.GetHealth(),tileSize,gradientRG(100,User.GetHealth()));
        DrawText(MainWindow,font,User.getGlobalBounds().left-tileSize*0.5,User.getGlobalBounds().top - tileSize*1,User.GetScore(),tileSize,sf::Color::Green);
        DrawText(MainWindow,font,NewZoom.getCenter().x-NewZoom.getSize().x/2,NewZoom.getCenter().y-NewZoom.getSize().y/2,FPS,std::min(tileSize*NewZoom.getSize().x*NewZoom.getSize().y , static_cast<float>(tileSize*2)) );
        
        if (Frames%4==0)
        {
            am.update("Walking",EntitiesList[0].GetSprite());
        }
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
        MainWindow.draw(EntitiesList[0]);
        // DrawContainingBox(MainWindow,User);
        // DrawContainingBoxInt(MainWindow,User);
        MainWindow.display();
        
    }
    return 0;
}
