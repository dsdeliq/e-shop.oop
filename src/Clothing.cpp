#include"Clothing.h"

Clothing::Clothing(const std::string& name, const Price& price,
             const Quantity& quantity, const Weight& weight):Product(name, price, quantity, weight, "clothing") {}



Jacket::Jacket(const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight):Clothing(name,price,quantity,weight){};

std::unique_ptr<Product> Jacket::clone() const {
    return std::make_unique<Jacket>(*this);
}

TShirt::TShirt(const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight):Clothing(name,price,quantity,weight){};

std::unique_ptr<Product> TShirt::clone() const {
    return std::make_unique<TShirt>(*this);
}

