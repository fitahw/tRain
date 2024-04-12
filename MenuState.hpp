#pragma once
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include <iostream>

class MenuState : public State {
public:
    // Classes with constructor with arguments  req#1
    MenuState(sf::RenderWindow& window) : State(window) {
        options.push_back("New Game");
        options.push_back("Load Game");
        options.push_back("Exit");

        font.loadFromFile("font/aoboshi.ttf");
        titleText.setFont(font);
        titleText.setString("penile - polish electronic near interstationary line emulator");
        titleText.setCharacterSize(70);

        for (int i = 0; i < options.size(); i++) {
            sf::Text text;
            text.setFont(font);
            text.setString(options[i]);
            text.setPosition(50, 100 + i * 50);
            texts.push_back(text);
        }

        sf::Text text;
        text.setFont(font);
        text.setString("made by Szymon Wiezowski, 2023");
        text.setPosition(500, 700);
        texts.push_back(text);

        camera = window.getView();
    }

    void update() {}

    void draw() override
    {
        window.clear(sf::Color(0, 100, 100, 255));

        window.draw(titleText);

        for (sf::Text& text : texts) {
            window.draw(text);
        }
    }

    int checkInput(sf::Event event) override {
        switch (event.type)
        {
        case(sf::Event::Closed):
            window.close();
            return -1;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            for (int i = 0; i < options.size(); i++) {
                sf::IntRect bounds(texts[i].getPosition().x, texts[i].getPosition().y, texts[i].getGlobalBounds().width, texts[i].getGlobalBounds().height);
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

                if (bounds.contains(pixelPos.x, pixelPos.y)) {
                    return i;
                }
            }
        }

        return -1;
    }

    void setCamera() {
        window.setView(camera);
    }

private:
    std::vector<sf::Text> texts;
    std::vector<std::string> options;
    sf::Font font;
    sf::Text titleText;
    sf::View camera;
};