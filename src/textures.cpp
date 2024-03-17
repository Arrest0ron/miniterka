#include "textures.h"


// void DrawFractal(sf::RenderWindow& window,double color1,double color2,int maxIterations){
//     int windowX = window.getSize().x/2;
//     int windowY = window.getSize().y;
//     for (int y = 0; y < windowY; ++y)
//     {
//         for (int x = 0; x < windowX; ++x)
//         {
//             std::complex<double> c(
//                 (x) * 5.0 / windowX / 6 ,
//                 (y) * 4.0 / windowY / 5
//                 // 10,10
//             );

//             std::complex<double> z =0;
//             int iterations = 0;


//             while (std::abs(z) < 2.0 && iterations < maxIterations)
//             {
//                 z = z * z + c;
//                 ++iterations;
//             }
            
//             sf::Color color = (iterations == maxIterations) ? sf::Color::Black : sf::Color(0, iterations * color1, iterations * color2);
//             sf::Vertex pixel(sf::Vector2f(x+windowX, y), color);
//             sf::Vertex pixel2(sf::Vector2f(windowX-x, y), color);
//             window.draw(&pixel, 1, sf::Points);
//             window.draw(&pixel2, 1, sf::Points);
//         }
//     }
// }