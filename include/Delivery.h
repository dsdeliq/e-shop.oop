#pragma once
#include <memory>
#include <string>

class Order;   

//  доставки
class Delivery {
public:
    virtual ~Delivery() = default;

    virtual double getCost(const Order& order) const = 0;
    virtual std::string getEstimatedTime() const = 0;
    virtual bool isAvailable(const Order& order) const = 0;
};

// Курьер 
class CourierDelivery final : public Delivery {
public:
    double getCost(const Order& order) const override;
    std::string getEstimatedTime() const override;
    bool isAvailable(const Order& order) const override;
};

// самовывоз 
class PickupDelivery final : public Delivery {
public:
    double getCost(const Order& order) const override;
    std::string getEstimatedTime() const override;
    bool isAvailable(const Order& order) const override;
};

// почта 
class PostDelivery final : public Delivery {
public:
    double getCost(const Order& order) const override;
    std::string getEstimatedTime() const override;
    bool isAvailable(const Order& order) const override;
};

