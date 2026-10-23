#include"Food.h"

Food::Food(const std::string& name, const Price& price,
             const Quantity& quantity, const Weight& weight):Product(name, price, quantity, weight, "food") {}

Bread::Bread(const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight):Food(name,price,quantity,weight){};

std::unique_ptr<Product> Bread::clone() const {
    return std::make_unique<Bread>(*this);
}

Milk::Milk(const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight):Food(name,price,quantity,weight){};

std::unique_ptr<Product> Milk::clone() const {
    return std::make_unique<Milk>(*this);
}

