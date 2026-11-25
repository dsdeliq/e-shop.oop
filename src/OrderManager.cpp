#include "OrderManager.h"
#include "Product.h"
#include <iostream>
#include <stdexcept>

OrderManager::OrderManager(int maxActiveOrders, int maxTotalActive)
    : maxActiveOrders_(maxActiveOrders), maxTotalActive_(maxTotalActive) {
    if (maxActiveOrders <= 0 || maxTotalActive <= 0) {
        throw std::invalid_argument("Order limits must be positive");
    }
}

int OrderManager::createOrder(const std::string& customer,
                              const std::vector<std::shared_ptr<Product>>& items,
                              const std::string& region) {
    // 1. проверка лимитов до любых изменений состояния
    if (getActiveOrderCount(customer) >= static_cast<std::size_t>(maxActiveOrders_)) {
        throw std::runtime_error("Превышен лимит активных заказов у клиента " + customer);
    }
    if (activeTotal_ >= static_cast<std::size_t>(maxTotalActive_)) {
        throw std::runtime_error("Превышен общий лимит активных заказов");
    }

    // 2. создаём заказ (если addItem бросит исключение, состояние менеджера не изменится)
    const int id = nextOrderId_;
    auto order = std::make_unique<Order>(id, customer, region);
    for (const auto& item : items) {
        order->addItem(item);
    }
    ++nextOrderId_;

    // 3. подписываемся на изменения статуса этого заказа
    order->subscribe(this);

    // 4. регистрируем: сначала обычный указатель, потом передаём владение
    ordersById_[id] = order.get();
    orders_.push_back(std::move(order));
    customers_.insert(customer);
    ++activeByCustomer_[customer];
    ++activeTotal_;

    return id;
}

Order* OrderManager::getOrder(int orderId) {
    auto it = ordersById_.find(orderId);
    return it == ordersById_.end() ? nullptr : it->second;
}

const Order* OrderManager::getOrder(int orderId) const {
    auto it = ordersById_.find(orderId);
    return it == ordersById_.end() ? nullptr : it->second;
}

std::size_t OrderManager::getActiveOrderCount(const std::string& customer) const {
    auto it = activeByCustomer_.find(customer);
    return it == activeByCustomer_.end() ? 0 : it->second;
}

bool OrderManager::updateStatus(int orderId, OrderStatus newStatus) {
    Order* order = getOrder(orderId);
    if (!order) {
        return false;                                            // заказа с таким id нет
    }
    if (order->getStatus() != newStatus &&
        !isTransitionAllowed(order->getStatus(), newStatus)) {
        return false;                                            // ненельзя такой  переход
    }
    order->setStatus(newStatus);                                 // заказ сам вызовет onOrderEvent
    return true;
}

// Вызывается заказом при каждой смене статуса
void OrderManager::onOrderEvent(const OrderEvent& event) {
    Order* order = getOrder(event.orderId);
    if (!order) {
        return;
    }

    // оплачен -> растёт оборот запоминаем сумму чтобы если что вернуть при отмне 
    if (event.newStatus == OrderStatus::Paid) {
        const double amount = order->getTotalPrice();
        paidAmount_[event.orderId] = amount;
        turnover_ += amount;
    }

    // отменён или доставлен -> заказ больше не активный, лимиты освобождаются
    if (event.newStatus == OrderStatus::Cancelled || event.newStatus == OrderStatus::Delivered) {
        auto it = activeByCustomer_.find(order->getCustomer());
        if (it != activeByCustomer_.end() && it->second > 0) {
            --it->second;
        }
        if (activeTotal_ > 0) {
            --activeTotal_;
        }
    }

    // отмена уже оплаченного заказа -> деньги возвращены, оборот уменьшается
    if (event.newStatus == OrderStatus::Cancelled) {
        auto paid = paidAmount_.find(event.orderId);
        if (paid != paidAmount_.end()) {
            turnover_ -= paid->second;
            paidAmount_.erase(paid);
        }
    }

    std::cout << "[OrderManager] заказ #" << event.orderId << ": "
              << toString(event.oldStatus) << " -> " << toString(event.newStatus)
              << ", оборот " << turnover_
              << ", активных у " << order->getCustomer() << ": "
              << getActiveOrderCount(order->getCustomer()) << std::endl;
}
