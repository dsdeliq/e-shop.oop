#pragma once
#include "Product.h"

class Electronics : public Product {
public:
    Electronics(const std::string& name, const Price& price,
                const Quantity& quantity, const Weight& weight);

protected:
    Electronics(const Electronics&) = default;
};

class Laptop : public Electronics {
public:
    Laptop(const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight);

    std::unique_ptr<Product> clone() const override;
};

class Smartphone : public Electronics {
public:
    Smartphone(const std::string& name, const Price& price,
               const Quantity& quantity, const Weight& weight);

    std::unique_ptr<Product> clone() const override;
};
