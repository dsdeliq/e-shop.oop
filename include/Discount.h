#pragma once
#include <memory>
#include <string>
#include <utility>
#include "Product.h"

class Order;

// ==================== Интерфейс стратегии скидки ====================
class Discount {
public:
    virtual ~Discount() = default;
    virtual void apply(Order& order) const = 0;
    virtual std::string getName() const = 0;   // для журнала discounts_ в Order
};

// ==================== Процентная скидка ====================

class PercentDiscount final : public Discount {
    double percent_;   // например, 10.0 = 10%
public:
    explicit PercentDiscount(double percent);      // percent должен быть от 0 до сотки
    void apply(Order& order) const override;
    std::string getName() const override;
};

// ==================== Кешбэк ====================
class CashbackDiscount final : public Discount {
    double percent_;
public:
    explicit CashbackDiscount(double percent);     // percent должен быть в от 0 до сотки
    void apply(Order& order) const override;
    std::string getName() const override;
};

// ==================== Бесплатная доставка ====================
class FreeDeliveryDiscount final : public Discount {
public:
    void apply(Order& order) const override;
    std::string getName() const override { return "FreeDelivery"; }
};

// ==================== Подарок ====================
class GiftDiscount final : public Discount {
    std::shared_ptr<Product> gift_;   // прототип подарка (клонируется при apply)
public:
    explicit GiftDiscount(std::shared_ptr<Product> gift); // странно было написано с какой то инициализацией прям в хедерсе хз че это
    void apply(Order& order) const override;
    std::string getName() const override;
};
