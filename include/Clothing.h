#pragma once
#include "Product.h"

class Clothing : public Product {   
public:
    Clothing( const std::string& name, const Price& price,
             const Quantity& quantity, const Weight& weight);

};

class Jacket : public Clothing {   
public:
    Jacket(const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight);
    std::unique_ptr<Product> clone() const override;

};

class TShirt : public Clothing {
public:
    TShirt( const std::string& name, const Price& price,
           const Quantity& quantity, const Weight& weight);
    std::unique_ptr<Product> clone() const override;

};

