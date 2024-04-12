#pragma once

#include "SFML/Graphics.hpp"
#include "ItemStack.hpp"
#include <queue>
#include <list>
#include "DeliveryJob.hpp"

class Player {
    sf::Texture* txt_player;
    sf::Sprite spr_player;

public:
    static const int startX = 60960;

    // Klasy z konstruktorem z argumentami, konstruktor kopiuj¹cy req#1
    Player(sf::Texture* txt_player);
    Player(sf::Texture* txt_player,
        std::list<ItemStack> inv,
        std::queue<Delivery> jobs,
        sf::Vector2f pos);
    ~Player();

    std::list<ItemStack> inventory;
    std::queue<Delivery> jobs;

    bool move(bool dir);

    void addItem(ItemStack& item);
    void printInventory();

    ItemStack& getItem(const std::string& name);
    void takeJob(std::string& station, std::string& wood, int needed);

    int getItemCount(const std::string& itemName);
    sf::Sprite getSprite() const;
    sf::Texture* getTexture() const;
};