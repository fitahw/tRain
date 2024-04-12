#pragma once

#include "SFML/Graphics.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"

class Game
{
	// Agregacja (kompozycja)	req#4
	// relacja z klasami State
	MenuState* menuState;
	GameState* gameState;
	State* currentState;

	sf::Vector2f viewSize;
	sf::RenderWindow window;
	sf::View camera;

	sf::Texture* txt_player;

public:
	// Klasy z konstruktorem z argumentami, konstruktor kopiuj¹cy req#1
	Game(World& world);
	~Game();

	void gameLoop();
};