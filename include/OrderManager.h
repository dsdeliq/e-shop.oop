#pragma once
#include <cstddef> // базовые сущности типа size t и тл
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "IObserver.h"
#include "Order.h"
#include "OrderEvent.h"

class Product;

class OrderManager final : public IObserver 
{
    std::vector<std::unique_ptr<Order>> orders_;                    
    std::unordered_map<int, Order*> ordersById_;                   
    std::unordered_set<std::string> customers_;                     
    std::unordered_map<std::string, std::size_t> activeByCustomer_; 
    std::unordered_map<int, double> paidAmount_;                    // сколько внесено в оборот по заказу
    std::size_t activeTotal_ = 0;                                   // активные заказы всех клиентов
    double turnover_ = 0.0;
    int maxActiveOrders_;      
    int maxTotalActive_;       // лимит нагрузки сколько активных заказов всего
    int nextOrderId_ = 1;

public:
    explicit OrderManager(int maxActiveOrders = 5, int maxTotalActive = 1000);

    // создать заказ и вернуть его айдишник 
    int createOrder(const std::string& customer,
                    const std::vector<std::shared_ptr<Product>>& items,
                    const std::string& region = "Москва");

    // 0, если заказа нет или переход статуса недопустим
    bool updateStatus(int orderId, OrderStatus newStatus);

    Order* getOrder(int orderId);
    const Order* getOrder(int orderId) const;
    const std::vector<std::unique_ptr<Order>>& getOrders() const { return orders_; }

    // инфа из объектов
    double getTurnover() const { return turnover_; } 
    std::size_t getCustomerCount() const { return customers_.size(); }
    std::size_t getActiveOrderCount() const { return activeTotal_; }
    std::size_t getActiveOrderCount(const std::string& customer) const;

    //  вызывается заказом при каждой смене статуса
    void onOrderEvent(const OrderEvent& event) override;
};
