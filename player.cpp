#include "player.hpp"
#include "game.hpp"

Player::Player(sf::Texture* p_txt)
{
    txt_player = p_txt;
    txt_player->loadFromFile("nig.png");
    spr_player.setTexture(*txt_player);
    spr_player.setPosition(startX, 768 * 0.88);

    // Kontenery sekwencyjne – zamiana tablic na kolejki  req#10
    Delivery start_jobs[1] = { Delivery("Oak Wood", 10, "uhhh station") };

    int arraySize = sizeof(start_jobs) / sizeof(start_jobs[0]);
    jobs = (std::queue<Delivery>)std::deque<Delivery>(start_jobs, start_jobs + arraySize);

    // Kontenery sekwencyjne – zamiana tablic na listy req#10
    ItemStack start_items[1] = { ItemFactory::createItem<Axe>(1) };
    arraySize = sizeof(start_items) / sizeof(start_items[0]);
    inventory = std::list<ItemStack>(start_items, start_items + arraySize);

}

Player::Player(sf::Texture* txt_player, std::list<ItemStack> inv, std::queue<Delivery> jobs, sf::Vector2f pos) :
    inventory(inv), jobs(jobs), txt_player(txt_player) {
    spr_player.setTexture(*txt_player);
    spr_player.setPosition(pos);
}

Player::~Player()
{
    // nie usuwamy pointera, bedzie potrzebny
}

bool Player::move(bool dir)
{
    switch (dir)
    {
    case(0):
        spr_player.setTextureRect(sf::IntRect(0, 0, 135, 55));
        if (spr_player.getPosition().x < 60700)
        {
            spr_player.move(20, 0);
            return 0;
        }
        else return 1; // 1 == PLAYER OUTSIDE OF BOUNDS ON RIGHT SIDE
    case(1):
        spr_player.setTextureRect(sf::IntRect(135, 0, -135, 55));
        if (spr_player.getPosition().x > 60000)
        {
            spr_player.move(-20, 0);
            return 0;
        }
        else return 1; // 1 == PLAYER OUTSIDE OF BOUNDS ON LEFT SIDE
    }
}

sf::Sprite Player::getSprite() const{return spr_player;}
sf::Texture* Player::getTexture() const{return txt_player;}

void Player::addItem(ItemStack& item)
{
    for (auto& stack : inventory) {
        if (*item.material == *stack.material) { // use of overloaded comparison operator
            stack = stack + item.getQuantity(); // use of overloaded addition operator
            return;
        }
    }

    if (item.getQuantity() > 0) {
        inventory.push_back(item);
    }
}

void Player::printInventory()
{
    for (auto& item : inventory)
        std::cout << item << std::endl; // use of overloaded output stream operator
}

ItemStack& Player::getItem(const std::string& itemName) {
    for (auto& item : inventory)
    {
        if (item.material->getName() == itemName)
        {
            return item;
        }
    }
    throw std::runtime_error("Item not found: " + itemName);
}

int Player::getItemCount(const std::string& itemName) {
    try {
        ItemStack item = getItem(itemName);
        return item.getQuantity();
    }
    catch (const std::runtime_error& e) {
        return 0;
    }
}

void Player::takeJob(std::string& station_name, std::string& wood_type, int needed)
{
    jobs.push(Delivery(wood_type, needed, station_name));
}