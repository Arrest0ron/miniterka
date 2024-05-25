#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

//extern int tileSize;
int menu()
{
    // Загрузка изображения
    sf::Texture texture;
    if (!texture.loadFromFile("phonk.png"))
    {
        // Обработка ошибки загрузки изображения
        return 1;
    }

    // Создание объекта Sprite для отображения изображения
    sf::Sprite sprite(texture);

    // Загрузка изображения для кнопки
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("button.png"))
    {
        // Обработка ошибки загрузки изображения
        return 1;
    }

    // Создание объекта Sprite для кнопки
    sf::Sprite buttonSprite(buttonTexture);
    buttonSprite.setPosition(400, 600);
    buttonSprite.setScale(1.6f, 1.6f); // Установка размера кнопки

    // Создание окна
    sf::RenderWindow window(sf::VideoMode(900, 900), "SFML Window");

    // Флаг для отслеживания, было ли открыто новое окно
    bool newWindowOpened = false;

    // Основной цикл приложения
    while (window.isOpen())
    {
        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // Проверка, была ли нажата кнопка
                if (buttonSprite.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                {
                    // Открытие нового окна
                    sf::RenderWindow newWindow(sf::VideoMode(900, 900), "New Window");
                    newWindowOpened = true;
                    window.close();
                    // Основной цикл нового окна
                    while (newWindow.isOpen())
                    {
                        sf::Event newEvent;
                        while (newWindow.pollEvent(newEvent))
                        {
                            if (newEvent.type == sf::Event::Closed)
                                newWindow.close();
                        }

                        newWindow.clear();
                        // Отрисовка содержимого нового окна
                        newWindow.display();
                    }
                }
            }
        }

        // Очистка окна
        window.clear();

        // Отрисовка спрайта и кнопки
        window.draw(sprite);
        window.draw(buttonSprite);

        // Отображение окна
        window.display();
    }

    return 0;
}
