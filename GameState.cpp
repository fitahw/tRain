#include <fstream>
#include <sstream>
#include "GameState.hpp"
#include <random>

GameState::GameState(sf::RenderWindow& window, Player& player, World& world) :
    State(window), m_player(player), m_world(world) {

    font.loadFromFile("font/aoboshi.ttf");
    for (size_t i = 0; i < 3; i++)
    {
        sf::Text t;
        t.setFont(font);
        t.setFillColor(sf::Color::White);
        t.setCharacterSize(30);
        t.move(player.getSprite().getPosition().x - 1000, 150 + i*50);
        texts[i] = t;
    }

    currentJobText.setFont(font);
    currentJobText.setFillColor(sf::Color::White);
    currentJobText.setCharacterSize(30);
    currentJobText.setOutlineColor(sf::Color(0, 0, 0, 128));
    currentJobText.setOutlineThickness(5);
    currentJobText.move(player.getSprite().getPosition().x - 500, 150);

    sf::Image s;
    s.loadFromFile("font/h.png");
    sf::Vector2u ss = s.getSize();
    sf::Cursor cursor;
    cursor.loadFromPixels(s.getPixelsPtr(), ss, sf::Vector2u(0, 0));
    window.setMouseCursor(cursor);
}

// Zapis (serializacja) i odczyt (deserializacja) obiektów do/z pliku csv req#7
void GameState::saveGame() {
    std::ofstream file(SAVE_FILENAME);
    if (!file.is_open()) {
        printf("Error opening file for writing.\n");
        return;
    }

    int playerX = m_player.getSprite().getPosition().x;
    int biomeX = Player::startX - playerX;
    file << playerX << "," << biomeX << std::endl;

    int itemsCount = m_player.inventory.size();
    file << itemsCount << std::endl;

    for (const auto& item : m_player.inventory) {
        file << item << std::endl;
    }

    int jobsCount = m_player.jobs.size();
    file << jobsCount << std::endl;

    // copy queue
    std::queue<Delivery> qcopy = m_player.jobs;
    // iterate queue
    while (!qcopy.empty()) {
        file << qcopy.front() << std::endl;
        qcopy.pop();
    }

    file.close();

    printf("GameData saved\n");
}

void GameState::loadGame() {
    std::ifstream file(SAVE_FILENAME);

    if (!file.is_open()) {
        std::cout << "Error opening file for reading." << std::endl;
        return;
    }

    int playerX, biomeX;
    std::string line;
    // Read player position
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string posX, biomePosX;
        if (std::getline(iss, posX, ',') && std::getline(iss, biomePosX)) {
            playerX = std::stoi(posX);
            biomeX = std::stoi(biomePosX);
        }
    }

    std::list<ItemStack> inventory;
    // Read items count
    if (std::getline(file, line)) {
        int itemsCount = std::stoi(line);

        // Read each item
        for (int i = 0; i < itemsCount && i < 100; ++i) {
            if (std::getline(file, line)) {
                std::istringstream isss(line);
                ItemStack item;
                isss >> item;
                inventory.push_back(item);
            }
        }
    }

    std::queue<Delivery> jobs;
    // Read jobs count
    if (std::getline(file, line)) {
        int jobsCount = std::stoi(line);
        // Read each job
        for (int i = 0; i < jobsCount; ++i) {
            if (std::getline(file, line)) {
                std::istringstream isss(line);
                Delivery job;
                isss >> job;

                jobs.push(job);
            }
        }
    }

    file.close();

    m_player = Player(m_player.getTexture(), inventory, jobs, sf::Vector2f(playerX, m_player.getSprite().getPosition().y));
    m_world.moveBiomes(biomeX, 0);

    printf("Gamedata loaded\n");
}

int GameState::inForest()
{
    // Kontenery sekwencyjne – zamiana tablic na wektory  req#10
    std::vector<Forest> forests(m_world.biomes.forests,
        m_world.biomes.forests + sizeof m_world.biomes.forests
        / sizeof m_world.biomes.forests[0]);

    for (int i = 0; i < forests.size(); i++)
    {
        sf::Sprite f = forests[i].sprite;
        if (m_player.getSprite().getPosition().x >= f.getGlobalBounds().left &&
            m_player.getSprite().getPosition().x <= f.getGlobalBounds().left + f.getLocalBounds().width) {
            return i;
        }
    }
    return -1;
}

int GameState::checkInput(sf::Event event) {
    int fi = -1, si = -1, changeMenu = -1;

    // lambda function  req#12
    auto keyPressCb = [&](sf::Keyboard::Key key) {
        switch (event.key.code)
        {
        case (sf::Keyboard::S):
            saveGame();
            break;
        case (sf::Keyboard::D):
            if (m_player.move(0) != 0)
            {
                m_world.moveBiomes(-40, 0);
            }
            break;
        case(sf::Keyboard::A):
            if (m_player.move(1) != 0)
            {
                m_world.moveBiomes(40, 0);
            }
            break;
        case(sf::Keyboard::E):
            fi = inForest();
            std::cout << fi << "\n";
            if (fi > -1 && fi <= 3)
            {
                Forest forest = m_world.biomes.forests[fi];
                ItemStack wood;
                if (forest.woodType == "Oak Wood")
                    wood = ItemFactory::createItem<OakWood>(forest.woodCount);
                else if (forest.woodType == "Spruce Wood")
                    wood = ItemFactory::createItem<SpruceWood>(forest.woodCount);
                else if (forest.woodType == "Birch Wood")
                    wood = ItemFactory::createItem<BirchWood>(forest.woodCount);
                m_player.addItem(wood);
                m_player.printInventory();
                break;
            }
            si = inStation();
            if (si > -1 && si <= 3)
            {
                Station station = m_world.biomes.stations[si];
                Delivery job = m_player.jobs.front();

                if (job.station_name == station.name) {
                    try {
                        ItemStack &wood = m_player.getItem(job.wood_type);
                        if (wood.getQuantity() >= job.wood_needed) {
                            wood = wood - job.wood_needed;
                            // complete job
                            m_player.jobs.pop();

                            if (m_player.jobs.size() == 0) {
                                // Generate random jobs
                                std::random_device rd;
                                std::mt19937 gen(rd());
                                std::uniform_int_distribution<int> forestDist(0, 2);
                                std::uniform_int_distribution<int> stationDist(0, 2);
                                std::uniform_int_distribution<int> woodNeededDist(1, 100);

                                int numJobs = 5;
                                for (int i = 0; i < numJobs; ++i) {
                                    int randomForestIndex = forestDist(gen);
                                    int randomStationIndex = stationDist(gen);
                                    std::string& woodType = m_world.biomes.forests[randomForestIndex].woodType;
                                    std::string& stationName = m_world.biomes.stations[randomStationIndex].name;
                                    int woodNeeded = woodNeededDist(gen);

                                    m_player.takeJob(stationName, woodType, woodNeeded);
                                }
                            }
                        }
                    }
                    catch (const std::runtime_error& e) {
                        std::cout << "Error: " << e.what() << std::endl;
                    }
                }
            }
            break;
        case(sf::Keyboard::Escape):
            changeMenu = 1;
            break;
        }
    };

    switch (event.type)
    {
    case (sf::Event::KeyPressed):
        keyPressCb(event.key.code);
        break;
    case(sf::Event::Closed):
        window.close();
        break;
    }

    return changeMenu;
}

// Prze³adowanie funkcji    req#2
void GameState::draw(sf::Sprite sprite) {
    window.draw(sprite);
}

void GameState::draw(sf::Text text) {
    window.draw(text);
}

void GameState::draw(std::string biomeName) {
    if (biomeName == "world") {
        window.draw(m_world.biomes.world);
    }
    else if (biomeName == "forests") {
        for (auto& f : m_world.biomes.forests)
            window.draw(f.sprite);
    }
    else if (biomeName == "stations") {
        for (auto& s : m_world.biomes.stations)
            window.draw(s.sprite);
    }
}

void GameState::draw() {
    draw("world");
    draw("forests");
    draw("stations");

    draw(texts[0]);
    draw(texts[1]);
    draw(texts[2]);
    draw(currentJobText);

    draw(m_player.getSprite());
}

void GameState::setCamera() {
    sf::Vector2f viewSize;
    sf::View camera;
    viewSize.x += 1200;
    viewSize.y += 768;

    camera.setSize(viewSize);
    camera.move(60000, 0);
    window.setView(camera);
}

int GameState::inStation()
{
    // Sequential containers - converting arrays into vectors   req#10
    std::vector<Station> stations(m_world.biomes.stations,
        m_world.biomes.stations + sizeof m_world.biomes.stations
        / sizeof m_world.biomes.stations[0]);

    for (int i = 0; i < stations.size(); i++)
    {
        sf::Sprite st = stations[i].sprite;
        if (m_player.getSprite().getPosition().x >= st.getGlobalBounds().left &&
            m_player.getSprite().getPosition().x <= st.getGlobalBounds().left + st.getLocalBounds().width) {
            return i;
        }
    }
    return -1;
}


void GameState::update() {
    std::vector<std::string> woods = { "Oak Wood", "Spruce Wood", "Birch Wood" };
    std::vector<int> woodCount(woods.size(), 0); // Initialize count vector with zeros

    if (m_player.jobs.size() > 0) {
        Delivery job = m_player.jobs.front();

        for (size_t i = 0; i < woods.size(); ++i) {
            if (job.wood_type == woods[i]) {
                woodCount[i] = job.wood_needed;
                break;
            }
        }

        std::shared_ptr<Material> ptr = ItemFactory::get()[job.wood_type];
        currentJobText.setString("Deliver " + ptr->getName() + " x" + std::to_string(job.wood_needed) + " to " + job.station_name);
    }

    std::vector<int> itemCount(woods.size(), 0);

    for (size_t i = 0; i < woods.size(); ++i) {
        itemCount[i] = m_player.getItemCount(woods[i]);
        texts[i].setString(woods[i] + ": " + std::to_string(itemCount[i]) + "/" + std::to_string(woodCount[i]));
    }
}

void GameState::newGame() {
    m_player = Player(m_player.getTexture());
    m_world.setDefaultBiomes();
}