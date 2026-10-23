#include "Product.h"
#include "Warehouse.h"
#include <stdexcept>
#include <utility>
int Product::nextId = 1;
Product::Product(const std::string& name, const Price& price,
                 const Quantity& quantity, const Weight& weight,
                 const std::string& category)
    : id_(nextId++),
      name_(name),
      price_(price),
      quantity_(quantity),
      weight_(weight),
      category_(category) {}

Product::Product(const Product& other)
    : id_(other.id_),
      name_(other.name_),
      price_(other.price_),
      quantity_(other.quantity_),
      weight_(other.weight_),
      category_(other.category_),
      owner_(nullptr) {}

void Product::setOwner(Warehouse* owner) noexcept {//без констр и оп присв не коп об
    owner_ = owner;
}
int Product::getId()const{
    return id_;
}    
const std::string& Product::getName() const { 
    return name_;
 }
const Price& Product::getPrice() const { 
    return price_;
 }
const Quantity& Product::getQuantity() const { 
    return quantity_;
 }
const Weight& Product::getWeight() const { 
    return weight_;
 }
const std::string& Product::getCategory() const { 
    return category_;
 } 
 void Product::setPrice(const Price& newPrice) {
    price_ = newPrice;
}
 
void Product::changeQuantity(int delta) {
    quantity_.change(delta); 
}    

void Product::transferTo(Warehouse& other) {
    if (owner_ == &other) {
        return;
    }

    if (owner_ == nullptr) {
        throw std::logic_error("Product has no owner warehouse");
    }

    if (other.size() >= other.maxSize()) {
        throw std::overflow_error("Warehouse is full");
    }

    Warehouse* source = owner_;
    auto product = source->extractById(id_);
    other.addProduct(std::move(product));
}

bool Price::operator==(const Price& other) const {
    return value_ == other.value_;
}
