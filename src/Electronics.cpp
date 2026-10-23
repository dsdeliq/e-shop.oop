#include "Electronics.h"


Electronics::Electronics(const std::string& name, const Price& price,
                         const Quantity& quantity, const Weight& weight)
    : Product(name, price, quantity, weight, "electronics") {}


Laptop::Laptop(const std::string& name, const Price& price,
       const Quantity& quantity, const Weight& weight)
    : Electronics(name, price, quantity, weight) {}
 
std::unique_ptr<Product> Laptop::clone() const {
    return std::make_unique<Laptop>(*this);
}

Smartphone::Smartphone(const std::string& name, const Price& price,
       const Quantity& quantity, const Weight& weight)
    : Electronics(name, price, quantity, weight) {}
 
std::unique_ptr<Product> Smartphone::clone() const {
    return std::make_unique<Smartphone>(*this);
}

