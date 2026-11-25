#pragma once
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "IObservable.h"
#include "OrderStatus.h"
#include "Product.h"

class Delivery;
class Discount;

class Order : public IObservable {
    int id_;
    std::string customer_;
    std::string region_;
    std::vector<std::shared_ptr<Product>> items_;
    OrderStatus status_;
    std::shared_ptr<Delivery> delivery_;
    std::vector<std::string> discounts_;          // журнал применённых акций (имена)
    std::vector<IObserver*> observers_;
    double discountAmount_ = 0;
    double cashback_ = 0;
    bool freeDelivery_ = false;

public:
    // --- конструирование ---
    Order(int id, std::string customer, std::string region = "Москва");

    // Глубокое копирование товаров; наблюдатели НЕ копируются
    Order(const Order& other);
    Order& operator=(const Order& other);
    Order(Order&&) noexcept = default;
    Order& operator=(Order&&) noexcept = default;
    ~Order() override = default;

    // --- данные ---
    int getId() const { return id_; }
    const std::string& getCustomer() const { return customer_; }
    const std::string& getRegion() const { return region_; }
    OrderStatus getStatus() const { return status_; }

    const std::vector<std::shared_ptr<Product>>& getItems() const { return items_; }
    std::shared_ptr<Delivery> getDelivery() const { return delivery_; }
    const std::vector<std::string>& getDiscounts() const { return discounts_; }

    // --- товары (только в статусе Pending) ---
    void addItem(std::shared_ptr<Product> item);
    bool removeItem(int productId);

    // --- расчёты ---
    double getItemsTotal() const;       // сумма товаров без скидок
    double getTotalWeight() const;
    double getDiscountAmount() const { return discountAmount_; }
    double getCashback() const { return cashback_; }
    bool   isDeliveryFree() const { return freeDelivery_; }
    double getDeliveryCost() const;     // 0, если доставки нет или она бесплатна
    double getTotalPrice() const;       // max(0, товары - скидка) + доставка

    //  мутаторы для акций (вызываются из Discount::apply) ---
    void addDiscountAmount(double v);
    void addCashback(double v) { cashback_ += v; }
    void setFreeDelivery(bool f) { freeDelivery_ = f; }

    // --- применение стратегий 
    void applyDiscount(const Discount& d);
    // Бросает std::invalid_argument, если способ доставки недоступен для этого заказа
    void setDelivery(std::shared_ptr<Delivery> d);

    // Смена статуса с проверкой перехода и уведомлением наблюдателей
    void setStatus(OrderStatus s);

    // --- IObservable
    void subscribe(IObserver* obs) override;
    void unsubscribe(IObserver* obs) override;
    void notify(const OrderEvent& event) override;

    // --- перегрузка операторов 
    // 1) order + discount -> новый Order с применённой скидкой
    Order operator+(const Discount& d) const; 

    Order operator+(const Product& item) const; // так же надо ордер + продукт 
    Order operator-(int productId) const; // новый заказ с таваром с айдишником 
    Order& operator+=(const Discount& d); // применить скидку к самому заказу 

    // delivery << order -> назначить доставку (эквивалент setDelivery)
    friend void operator<<(const std::shared_ptr<Delivery>& d, Order& order);
    // вывод заказа в поток
    friend std::ostream& operator<<(std::ostream& os, const Order& order);

private:
    void ensurePending(const char* action) const;
};
