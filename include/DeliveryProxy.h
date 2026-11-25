#pragma once
#include "Delivery.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class Order;   // 

class DeliveryProxy final : public Delivery {
    std::shared_ptr<Delivery> real_;
    std::shared_ptr<Delivery> fallback_;                    // запасной способ доставки 
    std::unordered_set<std::string> allowedRegions_;        // список для регионов, нельзя сувать повторки 
    
    struct CacheEntry 
    { 
        double weight; // вес заказа на момент расчёта
        double cost;  // посчитанная стоимость доставки
    };

    mutable std::unordered_map<int, CacheEntry> cache_;     // кеш по id заказа

    // какой способ реально используется для этого заказа
    const Delivery& pick(const Order& order) const;
    bool regionAllowed(const Order& order) const;

public:
    explicit DeliveryProxy(std::shared_ptr<Delivery> real, std::unordered_set<std::string> allowedRegions = {},
        std::shared_ptr<Delivery> fallback = nullptr)
        : real_(std::move(real)),
          fallback_(std::move(fallback)),
          allowedRegions_(std::move(allowedRegions)) {} // три параметра по тз типа проверка региона и переклбчение на альт 

    double getCost(const Order& order) const override;
    std::string getEstimatedTime() const override { return real_->getEstimatedTime(); }
    bool isAvailable(const Order& order) const override;
};
