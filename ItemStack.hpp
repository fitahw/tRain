#ifndef ITEMSTACK_H
#define ITEMSTACK_H

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include "Material.hpp"

class ItemFactory;

class ItemStack {
public:
    ItemStack() : quantity(0) {}
    // Konstruktor kopiujacy   req#1
    ItemStack(const ItemStack& other)
        : material(other.material), quantity(other.quantity) {}

    std::shared_ptr<Material> material;

    int getQuantity() const;

    // Przeci¹¿anie operatorów: arytmetyczny, tablicowy, przypisania, porównania req#5
    friend ItemStack operator+(ItemStack& item, int q);
    friend ItemStack operator-(ItemStack& item, int q);
    ItemStack& operator=(const ItemStack& other) {
        if (this != &other) {
            material = other.material;
            quantity = other.quantity;
        }
        return *this;
    }
    bool operator==(const ItemStack& other) const { return material == other.material && quantity == other.quantity; }
    bool operator!=(const ItemStack& other) const { return !(*this == other); }

    // 3.Funkcje zaprzyjaŸnione, klasy zaprzyjaŸnione   req#3
    friend ::ItemFactory;
    // Streams - operators >>, <<    req#6
    friend std::istream& operator>>(std::istream& is, ItemStack& stack);
    friend std::ostream& operator<<(std::ostream& os, const ItemStack& stack);

private:
    int quantity;

    ItemStack(std::shared_ptr<Material> material, int quantity);
};

// Agregacja (kompozycja)	req#4
// Factory pattern
class ItemFactory {
    // Kontenery asocjacyjne – zastosowanie kontenera map req#11
    std::map<std::string, std::shared_ptr<Material>> mats;

    ItemFactory() {
        mats.insert({ "Oak Wood", std::make_shared<OakWood>() });
        mats.insert({ "Spruce Wood", std::make_shared<SpruceWood>() });
        mats.insert({ "Birch Wood", std::make_shared<BirchWood>() });
        mats.insert({ "Axe", std::make_shared<Axe>() });
    }

public:
    template<typename T> static ItemStack createItem(int quantity);

    static ItemFactory& get() {
        static ItemFactory instance;
        return instance;
    }

    // Przeci¹¿anie operatorów: tablicowy    req#5
    std::shared_ptr<Material> operator[](std::string index) {
        std::shared_ptr<Material> mat = mats[index];
        return mat;
    }
};
#endif // ITEMSTACK_H