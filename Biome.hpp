#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>

class Station {
public:
    std::string name;
    sf::Sprite sprite;
    sf::Texture txt;

    Station(const std::string& name) : name(name) {
        txt.loadFromFile("imgs/" + name + ".png");
        sprite.setTexture(txt);
    }
};

class Forest {
public:
    std::string woodType;
    int woodCount;
    std::string name;
    sf::Sprite sprite;
    sf::Texture txt;

    Forest(const std::string& woodType, const std::string& name, int woodCount) : woodType(woodType), name(name), woodCount(woodCount) {
        txt.loadFromFile("imgs/" + name + ".png");
        sprite.setTexture(txt);
    }
};