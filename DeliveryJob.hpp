#pragma once

#include <string>
#include <sstream>
#include "Material.hpp"

struct Delivery
{
    std::string station_name;
    std::string wood_type;
    int wood_needed;

    Delivery() {}
    Delivery(std::string wood, int wood_needed, std::string station) :
        wood_type(wood), wood_needed(wood_needed), station_name(station) {}

    friend std::ostream& operator<<(std::ostream& os, const Delivery& job) {
        os << job.station_name << "," << job.wood_type
            << "," << job.wood_needed;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Delivery& job) {
        std::string line;
        std::getline(is, line);

        std::stringstream ss(line);
        std::string value;

        std::getline(ss, value, ',');
        job.station_name = value;

        std::getline(ss, value, ',');
        job.wood_type = value;

        std::getline(ss, value, ',');
        job.wood_needed = std::stoi(value);
        return is;
    }
};