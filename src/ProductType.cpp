#include "ProductType.h"

std::unordered_map<std::string, std::shared_ptr<ProductType>> ProductType::pool_;

ProductType::ProductType(const std::string& category, double baseWeight, double basePrice)
    : category_(category), baseWeight_(baseWeight), basePrice_(basePrice) {}

std::shared_ptr<const ProductType> ProductType::getType(const std::string& category) {
    auto it = pool_.find(category);
    if (it != pool_.end()) {
        return it->second;
    }

    // Базовые значения категории (общие для всех товаров категории)
    double weight = 1.0;
    double price = 0.0;
    if (category == "electronics")      { weight = 1.0; price = 1000.0; }
    else if (category == "clothing")    { weight = 0.3; price = 50.0; }
    else if (category == "food")        { weight = 1.0; price = 10.0; }

    auto type = std::make_shared<ProductType>(category, weight, price);
    pool_.emplace(category, type);
    return type;
}

std::size_t ProductType::poolSize() { return pool_.size(); }

const std::string& ProductType::getCategory() const { return category_; }
double ProductType::getBaseWeight() const { return baseWeight_; }
double ProductType::getBasePrice() const { return basePrice_; }
