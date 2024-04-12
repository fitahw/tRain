#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "State.hpp"
#include "player.hpp"
#include "world.hpp"

#define SAVE_FILENAME "gameData.csv"

class GameState : public State {
public:
    GameState(sf::RenderWindow& window, Player& player, World& world);

    // Function overloads   req#2
    void draw(sf::Sprite sprite);
    void draw(sf::Text text);
    void draw(std::string biomeName);
    void draw();

    void update();
    int checkInput(sf::Event event);
    void setCamera();

    int inStation();
    int inForest();

    void saveGame();
    void loadGame();
    void newGame();

private:
    Player m_player;
    World m_world;

    sf::Font font;
    sf::Text texts[3];
    sf::Text currentJobText;
};