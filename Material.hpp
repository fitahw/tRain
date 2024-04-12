#pragma once
#include <iostream>
#include <string>

// Polimorfizm - przes³anianie metod w klasach pochodnych, 
// przekazywanie objektów pochodnych w parametrach typu bazowego
class Material
{
public:
	Material();
	virtual ~Material();

	virtual std::string getName() const = 0;
	virtual bool operator==(const Material& other) const = 0;
};


struct BaseMaterial : virtual public Material
{
	std::string getName() const override {
		return "None";
	}

	bool operator==(const Material& other) const {
		return getName() == other.getName();
	}
};

struct OakWood : public BaseMaterial
{
	std::string getName() const override
	{
		return "Oak Wood";
	}
};

struct SpruceWood : public BaseMaterial
{
	std::string getName() const override
	{
		return "Spruce Wood";
	}
};

struct BirchWood : public BaseMaterial
{
	std::string getName() const override
	{
		return "Birch Wood";
	}
};

struct Axe : public BaseMaterial
{
	std::string getName() const override
	{
		return "Axe";
	}
};
