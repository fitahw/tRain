#include <iostream>
#include <string>
#include <sstream>
#include "ItemStack.hpp"

ItemStack::ItemStack(std::shared_ptr<Material> p_material, int p_quantity) :
	material(std::move(p_material)), quantity(p_quantity)
{
}

int ItemStack::getQuantity() const { return quantity; }

template<typename T> ItemStack ItemFactory::createItem(int quantity) {
	std::shared_ptr<T> mat = std::make_shared<T>();
	return ItemStack(std::move(mat), quantity);
}

//// OPERATORS OVERLOADING ////
ItemStack operator+(ItemStack& item, int q) { item.quantity += q; return item; }
ItemStack operator-(ItemStack& item, int q) { item.quantity -= q; return item; }

// Streams - operators >>, <<    req#6
std::istream& operator>>(std::istream& is, ItemStack& stack) {
	std::string line;
	std::getline(is, line);

	std::stringstream ss(line);
	std::string value;

	std::getline(ss, value, ',');
	stack.material = ItemFactory::get()[value];

	std::getline(ss, value, ',');
	stack.quantity = std::stof(value);

	return is;
}

std::ostream& operator<<(std::ostream& os, const ItemStack& item) {
	std::string fullname = item.material->getName();
	os << item.material->getName() << "," << item.quantity;
	return os;
}


// template explicit initialization so that linker is happy...
template ItemStack ItemFactory::createItem<OakWood>(int);
template ItemStack ItemFactory::createItem<SpruceWood>(int);
template ItemStack ItemFactory::createItem<BirchWood>(int);
template ItemStack ItemFactory::createItem<Axe>(int);