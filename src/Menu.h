#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

//extern int tileSize;


int menu()
{
    // Загрузка изображения фона
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("phonk.png"))
    {
        // Обработка ошибки загрузки изображения
        return 1;
    }

    // Создание объекта Sprite для фона
    sf::Sprite backgroundSprite(backgroundTexture);

    // Загрузка изображения кнопки
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("button.png"))
    {
        
        return 1;
    }

    sf::Sprite buttonSprite(buttonTexture);
    buttonSprite.setPosition(400, 600);
    buttonSprite.setScale(1.6f , 1.6f); // Установка размера кнопки


    sf::RenderWindow window(sf::VideoMode(900, 900), "SFML Window");

    bool window2 = false;




    sf::Texture button1Texture, button2Texture, selectButtonTexture;
    if (!button1Texture.loadFromFile("button.png") || !button2Texture.loadFromFile("button.png") || !selectButtonTexture.loadFromFile("button.png"))
    {
        // Обработка ошибки загрузки изображений
        return 1;
    }

    // Создание объектов Sprite для кнопок в новом окне
    sf::Sprite button1Sprite(button1Sprite), button2Sprite(button2Texture), selectButtonSprite(selectButtonTexture);
    button1Sprite.setPosition(200, 600);
    button2Sprite.setPosition(450, 600);
    selectButtonSprite.setPosition(700, 600);

    //  был ли выбран персонаж
    bool characterSelected = false;
    std::string selectedCharacterFile = "";
    sf::Texture characterTexture;

    sf::Event event;

    sf::RenderWindow newWindow(sf::VideoMode(900, 900), "Character Selection");
    newWindow.setVisible(false);

    // Основной цикл 
    while (window.isOpen())
    {
        // Обработка событий
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                newWindow.close();
            }   
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {

                //  была ли нажата кнопка
                if (buttonSprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                {
                    // Открытие нового окна
                    std::cout << " here";
                    
                    newWindow.setVisible(true);
                    window2 = true;


                }

            }

        }
        while (window2 && newWindow.isOpen())
        {
            while (newWindow.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    newWindow.setVisible(false);
                    window2 = false;
                }
               


                else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "!";
                    // была ли нажата кнопка переключения персонажа
                    if (button1Sprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    {
                        if (!characterTexture.loadFromFile("char_01.png"))
                        {
                            return 1;
                        }
                        std::cout << "ch1 loaded.\n";
                        sf::Sprite characterSprite(characterTexture);
                        characterSprite.setPosition(300, 300);
                    }
                    else if (button2Sprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    {
                        if (!characterTexture.loadFromFile("char_02.png"))
                        {
                            return 1;
                        }
                        std::cout << "ch2 loaded.\n";
                        sf::Sprite characterSprite(characterTexture);
                        characterSprite.setPosition(300, 300);
                    }
                    else if (selectButtonSprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                    {
                        // Установка флага, что персонаж выбран
                        characterSelected = true;

                        // Сохранение имени файла выбранного персонажа
                        if (button1Sprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                        {
                            selectedCharacterFile = "char_01.png";
                        }
                        else if (button2Sprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                        {
                            selectedCharacterFile = "char_02.png";
                        }

                    }
                }
            }
            newWindow.clear();
            newWindow.draw(backgroundSprite);
            newWindow.draw(button1Sprite);
            newWindow.draw(button2Sprite);
            newWindow.display();
        }
        
        window.clear();

        // Отрисовка спрайта и кнопки в главном окне

        
        window.draw(backgroundSprite);
        window.draw(buttonSprite);
        window.display();

    
    }
    
    
        // Возвращаем статус выбора персонажа
    return characterSelected;
    
    // return 0;
}