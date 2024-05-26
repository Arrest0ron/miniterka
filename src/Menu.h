#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

//extern int tileSize;
//extern std::string player;

int menu()
{
    int num = 0;
    // Загрузка изображения фона
    sf::Texture backgroundTexture;
    std::string background1, background2,button1,button2,prefix,playerPrefix;
    background1 = "phonk.png";
    background2 = "phonk2.png";
    button1 = "button.png";
    button2 = "button1.png";
    #ifndef WIN32
        prefix = "/home/user/Documents/GitHub/miniterka/images/";
        playerPrefix = "players/";


    #endif

    if (!backgroundTexture.loadFromFile(prefix+background1))
    {
        // Обработка ошибки загрузки изображения
        return 1;
    }

    // Создание объекта Sprite для фона
    sf::Sprite backgroundSprite(backgroundTexture);

    // Загрузка изображения кнопки
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile(prefix+button1))
    {
        
        return 1;
    }

    sf::Sprite buttonSprite(buttonTexture);
    buttonSprite.setPosition(400, 600);
    buttonSprite.setScale(1.6f , 1.6f); // Установка размера кнопки


    sf::RenderWindow window(sf::VideoMode(900, 900), "SFML Window");

    bool window2 = false;




    sf::Texture button1Texture, button2Texture, selectButtonTexture;
    
    if (!button1Texture.loadFromFile(prefix+button1) || !button2Texture.loadFromFile(prefix+button1) || !selectButtonTexture.loadFromFile(prefix+button2))
    {
        // Обработка ошибки загрузки изображений
        return -1;
    }

    // Создание объектов Sprite для кнопок в новом окне
    // sf::Texture backgroundTexture2;
    // sf::Sprite backgroundSprite2(backgroundTexture2);
    sf::Sprite button1Sprite(button1Texture), button2Sprite(button2Texture), selectButtonSprite(selectButtonTexture);
    button1Sprite.setPosition(200, 600);
    button2Sprite.setPosition(700, 600);
    selectButtonSprite.setPosition(400, 600);
    button1Sprite.setScale(-0.8f,0.8f);
    button2Sprite.setScale(0.8f, 0.8f);
    selectButtonSprite.setScale(1.3f, 1.3f);

    //  был ли выбран персонаж
    bool characterSelected = false;
    std::string selectedCharacterFile = prefix+playerPrefix+"char_01.png";
    sf::Texture characterTexture;
    characterTexture.loadFromFile(prefix+playerPrefix+"char_01.png");
    
    sf::Event event;

    // sf::RenderWindow newWindow(sf::VideoMode(900, 900), "Character Selection");
    // newWindow.setVisible(false);
    // Загрузка изображения фона для второго окна
    // sf::Texture backgroundTexture2;
    //sf::Texture backgroundTexture2;
    sf::Texture backgroundTexture2;
    if (!backgroundTexture2.loadFromFile(background2))
    {
        // Обработка ошибки загрузки изображения
        return 1;
    }

    // Создание объекта Sprite для фона второго окна
    sf::Sprite backgroundSprite2(backgroundTexture2);
    // sf::Sprite backgroundSprite2(backgroundTexture2);
    // if (!backgroundTexture2.loadFromFile("phonk2.png"))
    // {
    //     // Обработка ошибки загрузки изображения
    //     return 1;
    // }

// Создание объекта Sprite для фона второго окна
//sf::Sprite backgroundSprite2(backgroundTexture2);
    // Основной цикл 
    while (window.isOpen())
    {
        // Обработка событий
        if (!window2)
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    
                }   
                else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {

                    //  была ли нажата кнопка
                    if (buttonSprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    {
                        // Открытие нового окна
                        std::cout << " here";
                        
                        // newWindow.setVisible(true);
                        window2 = true;


                    }

                }

            }
            window.clear();

            // Отрисовка спрайта и кнопки в главном окне

            
            window.draw(backgroundSprite);
            window.draw(buttonSprite);
            window.display();
            //window.close();
        }
            
        if (window2)
        {
            while (window.pollEvent(event))
            {
                // if (event.type == sf::Event::Closed)
                // {
                //     newWindow.setVisible(false);
                //     window2 = false;
                // }
                if (event.type == sf::Event::Closed)
                {

                    window2 = false;
                    window.close(); // Закрываем первое окно
                }
                    


                else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "!";
                    // была ли нажата кнопка переключения персонажа
                    if (button1Sprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    {
                        if (!characterTexture.loadFromFile(prefix+playerPrefix+"char_01.png"))
                        {
                            return -1;
                        }
                        std::cout << "ch1 loaded.\n";
                        num = 1;
                        // sf::Sprite characterSprite(characterTexture);
                        // characterSprite.setPosition(450, 450);
                        // characterSprite.setScale(1.6f, 1.6f);
                        // newWindow.draw(characterSprite);
                    }
                    //newWindow.display();
                    else if (button2Sprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    {
                        if (!characterTexture.loadFromFile(prefix+playerPrefix+"char_02.png"))
                        {
                            return -1;
                        }
                        std::cout << "ch2 loaded.\n";
                        num = 2;
                        // sf::Sprite characterSprite(characterTexture);
                        // characterSprite.setPosition(300, 300);
                        // characterSprite.setScale(1.6f, 1.6f);
                        // newWindow.draw(characterSprite);
                    }
                    // else if (selectButtonSprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    // {
                    //     // Установка флага, что персонаж выбран
                    //     characterSelected = true;

                    //     // Сохранение имени файла выбранного персонажа
                    //     if (button1Sprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    //     {
                    //         selectedCharacterFile = prefix+playerPrefix+"char_01.png";
                    //         std::cout << "Chosen 1";
                    //         num = 1;
                    //     }
                    //     else if (button2Sprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    //     {
                    //         selectedCharacterFile = prefix+playerPrefix+"char_02.png";
                    //         std::cout << "Chosen 2";
                    //         num = 2;
                    //     }
                    else if (selectButtonSprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    {
                        return num;
                        window.close();
                    }


                    // }
                }
            }
            window.clear();
            window.draw(backgroundSprite2);
            window.draw(button1Sprite);

            window.draw(button2Sprite);

            window.draw(selectButtonSprite);
            sf::IntRect charRect(0, 0, 16, 16); // Начальные координаты (0, 0), ширина 16, высота 16
    // Создание спрайта с выбранной областью
            sf::Sprite characterSprite(characterTexture, charRect);
            //sf::Sprite characterSprite(characterTexture);
            characterSprite.setPosition(420, 500);
            characterSprite.setScale(5.0f, 5.0f);
            window.draw(characterSprite);
            // newWindow.draw(characterSprite);
            window.display();
        }
    

    
    }
    
    return num;
    
    // return 0;
}