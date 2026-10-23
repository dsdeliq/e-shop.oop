#pragma once
#include <memory>
#include <string>
#include "Price.h"
#include "Quantity.h"
#include "Weight.h"

class Warehouse;  // forward declaration, чтобы избежать циклической зависимости

// Абстрактный базовый класс всех товаров
class Product {
    friend class Warehouse;                // Warehouse выставляет owner_ при добавлении
    static int nextId;
    int id_;
    std::string name_;
    Price price_;
    Quantity quantity_;
    Weight weight_;
    std::string category_;
    Warehouse* owner_ = nullptr; 
    void setOwner(Warehouse* owner) noexcept;
    
public:
    Product( const std::string& name, const Price& price,
            const Quantity& quantity, const Weight& weight,
            const std::string& category);
    virtual ~Product() = default;

    // Прототип: полиморфное копирование (нужно для глубокого копирования склада)
    virtual std::unique_ptr<Product> clone() const = 0;

    // Геттеры
    int getId() const;
    const std::string& getName() const;
    const Price& getPrice() const;
    const Quantity& getQuantity() const;
    const Weight& getWeight() const;
    const std::string& getCategory() const;

    // Действия товара
    void setPrice(const Price& newPrice);
    void changeQuantity(int delta);        // delta может быть < 0; итог не должен стать отрицательным
    void transferTo(Warehouse& other);     // переносит товар со склада-владельца на other

    bool operator==(const Price& other) const;
protected:
    // При клонировании склад-владелец исходного товара не копируется.
    Product(const Product& other);
};
