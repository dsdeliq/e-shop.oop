#include "DeliveryProxy.h"
#include "Order.h"      // order.getId(), getTotalWeight(), getRegion()
#include <iostream>

bool DeliveryProxy::regionAllowed(const Order& order) const {
    if (order.getRegion().empty()) return false;
    return allowedRegions_.empty() || allowedRegions_.count(order.getRegion()) > 0;
}

const Delivery& DeliveryProxy::pick(const Order& order) const {
    if (regionAllowed(order) && real_->isAvailable(order)) {
        return *real_;
    }
    if (fallback_ && fallback_->isAvailable(order)) {
        return *fallback_;
    }
    return *real_;
}

double DeliveryProxy::getCost(const Order& order) const {
    const double weight = order.getTotalWeight();
    auto it = cache_.find(order.getId());
    if (it != cache_.end() && it->second.weight == weight) {
        return it->second.cost;                    // кеш-хит
    }

    std::cout << "[Proxy] Calculating cost for order " << order.getId() << "\n";

    const double cost = pick(order).getCost(order);   // делегирование
    cache_[order.getId()] = {weight, cost};
    return cost;
}

bool DeliveryProxy::isAvailable(const Order& order) const {
    std::cout << "[Proxy] Checking availability for order " << order.getId()
              << " (region: " << order.getRegion() << ")\n";

    // 1. проверка региона
    if (!regionAllowed(order)) {
        std::cout << "[Proxy] Region '" << order.getRegion() << "' is not served\n";
        return false;
    }

    // 2. делегирование реальной стратегии (там проверка веса)
    if (real_->isAvailable(order)) {
        return true;
    }
    std::cout << "[Proxy] Real delivery unavailable for order " << order.getId() << "\n";

    // 3. запасной способ
    if (fallback_ && fallback_->isAvailable(order)) {
        std::cout << "[Proxy] Switching to fallback delivery\n";
        return true;
    }
    return false;
}
