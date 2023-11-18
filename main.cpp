#include <SFML/Graphics.hpp>
#include <iostream>
#include "ComplexPlane.h"
#include <ctime>
using namespace std;
int main()
{
    // Get the desktop resolution
    int desktopWidth = sf::VideoMode::getDesktopMode().width;
    int desktopHeight = sf::VideoMode::getDesktopMode().height;

    // Divide values by 2 for a smaller screen
    int windowWidth = desktopWidth / 2;
    int windowHeight = desktopHeight / 2;

    // Construct the RenderWindow
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Mandelbrot Set");

    // Construct the ComplexPlane
    ComplexPlane complexPlane(windowWidth, windowHeight);

    // Construct the Font and Text objects
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        cerr << "Error loading font." << endl;
        return -1;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // Begin the main loop
    while (window.isOpen())
    {
        // Handle Input segment
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    complexPlane.zoomOut();
                    complexPlane.setCenter(sf::Mouse::getPosition(window));
                    complexPlane.setState(ComplexPlane::State::CALCULATING);
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    complexPlane.zoomIn();
                    complexPlane.setCenter(sf::Mouse::getPosition(window));
                    complexPlane.setState(ComplexPlane::State::CALCULATING);
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                complexPlane.setMouseLocation(sf::Mouse::getPosition(window));
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        // Update Scene segment
        complexPlane.updateRender();
        complexPlane.loadText(text);

        // Draw Scene segment
        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}
