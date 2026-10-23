#pragma once
#include "Product.h"

class Food : public Product {   
public:
    Food( const std::string& name, const Price& price,
         const Quantity& quantity, const Weight& weight);

protected:
    Food(const Food&) = default;

};

class Bread : public Food { 
public:
    Bread(const std::string& name, const Price& price,
          const Quantity& quantity, const Weight& weight);
          
    std::unique_ptr<Product> clone() const override;


};

class Milk : public Food {   
public:
    Milk(const std::string& name, const Price& price,
         const Quantity& quantity, const Weight& weight);
    std::unique_ptr<Product> clone() const override;
};



