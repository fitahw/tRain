#include "game.hpp"
#include "world.hpp"
#include "player.hpp"

Game::Game(World& world)
{
    txt_player = new sf::Texture();
    Player player(txt_player);

    viewSize.x += 1200;
    viewSize.y += 768;
    window.create(sf::VideoMode(viewSize.x, viewSize.y), "poland train simulator 2023 premium");

    menuState = new MenuState(window);
    gameState = new GameState(window, player, world);
    currentState = menuState;

    window.display();
}

Game::~Game() {
    currentState = nullptr;
    delete menuState, gameState, txt_player;

    if (window.isOpen()) {
        window.close();
    }
}

void Game::gameLoop()
{
    sf::Clock clock;
    float timePerUpdate = 1.f / 60.f;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        if (time >= timePerUpdate) {
            if (clock.getElapsedTime().asMilliseconds() > 10) {
                currentState->update();

                sf::Event event;
                while (window.pollEvent(event))
                {
                    int i = currentState->checkInput(event);
                    if (i > -1 && currentState == menuState) {
                        switch (i) {
                        case 0: // new game
                            gameState->newGame();
                            gameState->setCamera();
                            currentState = gameState;
                            break;
                        case 1: // load game
                            gameState->loadGame();
                            gameState->setCamera();
                            currentState = gameState;
                            break;
                        case 2: // exit
                            window.close();
                            break;
                        }
                    }
                    else if (i == 1 && currentState == gameState) {
                        menuState->setCamera();
                        currentState = menuState;
                    }
                }
                clock.restart();
            }
        }

        currentState->draw();
        window.display();
    }
}