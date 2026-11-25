#pragma once
#include <memory>
#include <vector>
#include "Product.h"

// Итератор по товарам склада: скрывает unique_ptr, *it даёт Product&
class WarehouseIterator {
public:
    using Base = std::vector<std::unique_ptr<Product>>::iterator;

    WarehouseIterator(Base current, Base end);

    Product& operator*() const;
    WarehouseIterator& operator++();
    bool operator!=(const WarehouseIterator& other) const;
    
private:
    Base current_;
    Base end_;
};


// Константный итератор: для обхода склада без изменения товаров
class ConstWarehouseIterator {
public:
    using Base = std::vector<std::unique_ptr<Product>>::const_iterator;

    ConstWarehouseIterator(Base current, Base end);

    const Product& operator*() const;
    ConstWarehouseIterator& operator++();
    bool operator!=(const ConstWarehouseIterator& other) const;

private:
    Base current_;
    Base end_;
};