#pragma once

class State
{
public:
    // Klasy z konstruktorem z argumentami, konstruktor kopiuj�cy  req#1
    // Dziedziczenie � konstruktory, destruktory  req#8
    State() = default;
    State(sf::RenderWindow& window) : window(window) {}
    virtual ~State() = default;

    // Polimorfizm - przes�anianie metod w klasach pochodnych, 
    // przekazywanie objekt�w pochodnych w parametrach typu bazowego req#9
    virtual int checkInput(sf::Event event) = 0;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void setCamera() = 0;

protected:
    sf::RenderWindow& window;
};