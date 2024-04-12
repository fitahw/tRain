#pragma once

#include "SFML/Graphics.hpp"
#include "Biome.hpp"

class World
{
    sf::IntRect worldSize;
    sf::Texture txt_world;

    //void emmap(sf::Sprite& spr, std::string key);
public:
    struct Biomes {
        sf::Sprite world;
        Station stations[3] = {Station("uhhh station"), Station("stargrad station"), Station("newgrad station") };
        Forest forests[3] = {Forest("Oak Wood", "forest1", 10), Forest("Spruce Wood", "forest2", 5), Forest("Birch Wood", "forest3", 1)};
    } biomes;

    World();
    ~World() {}
    
    void setDefaultBiomes();
    void moveBiomes(int x, int y);
};