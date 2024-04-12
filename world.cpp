#include "world.hpp"

World::World()
{
    // SETUP WORLD SPRITE
    worldSize.top = 0;
    worldSize.left = 0;
    worldSize.width = 120000;
    worldSize.height = 768;
    txt_world.create(1200, 768);
    txt_world.setRepeated(true);
    txt_world.loadFromFile("bg.png");
    biomes.world.setTextureRect(worldSize);
    biomes.world.setTexture(txt_world);
    biomes.world.setOrigin(0, -100);

    //emmap(spr_station, "station");

    //emmap(spr_forest, "forest");
}

void World::setDefaultBiomes() {
    for (size_t i = 0; i < 3; i++)
    {
        biomes.stations[i].sprite.setPosition(60600 + i * 1200, 345);
    }

    for (size_t i = 0; i < 3; i++)
    {
        biomes.forests[i].sprite.setPosition(61400 - i * 1000, 420);
    }
}

void World::moveBiomes(int x, int y)
{
    biomes.world.move(x, y);
    for (auto& forest : biomes.forests)
        forest.sprite.move(x, y);
    for (auto& station : biomes.stations)
        station.sprite.move(x, y);
}