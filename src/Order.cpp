#include "Order.h"
#include "Delivery.h"
#include "Discount.h"
#include "OrderEvent.h"
#include "OrderStatus.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <ostream>
#include <utility>

//  Конструирование 

Order::Order(int id, std::string customer, std::string region)
    : id_(id),
      customer_(std::move(customer)),
      region_(std::move(region)),
      status_(OrderStatus::Pending) {}

// Глубокое копирование товаров через clone(); наблюдатели и скидки НЕ копируются
Order::Order(const Order& other)
    : id_(other.id_),
      customer_(other.customer_),
      region_(other.region_),
      status_(other.status_),
      delivery_(other.delivery_),          // shared_ptr — делим доставку
      discounts_(other.discounts_),        // журнал скидок (имена)
      discountAmount_(other.discountAmount_),
      cashback_(other.cashback_),
      freeDelivery_(other.freeDelivery_) {
    items_.reserve(other.items_.size());
    for (const auto& item : other.items_) {
        if (item) {
            items_.push_back(std::shared_ptr<Product>(item->clone().release()));
        }
    }
    // observers_ намеренно не копируем
}

Order& Order::operator=(const Order& other) {
    if (this == &other) return *this;

    id_ = other.id_;
    customer_ = other.customer_;
    region_ = other.region_;
    status_ = other.status_;
    delivery_ = other.delivery_;
    discounts_ = other.discounts_;
    discountAmount_ = other.discountAmount_;
    cashback_ = other.cashback_;
    freeDelivery_ = other.freeDelivery_;

    items_.clear();
    items_.reserve(other.items_.size());
    for (const auto& item : other.items_) {
        if (item) {
            items_.push_back(std::shared_ptr<Product>(item->clone().release()));
        }
    }
    // observers_ не трогаем
    return *this;
}

//  Товары 

void Order::ensurePending(const char* action) const {
    if (status_ != OrderStatus::Pending) {
        throw std::logic_error(std::string("Cannot ") + action +
                               ": order is not in Pending status");
    }
}

void Order::addItem(std::shared_ptr<Product> item) {
    ensurePending("add item");
    if (!item) throw std::invalid_argument("Cannot add null product");
    items_.push_back(std::move(item));
}

bool Order::removeItem(int productId) {
    ensurePending("remove item");
    auto it = std::find_if(items_.begin(), items_.end(),
                           [productId](const std::shared_ptr<Product>& p) {
                               return p && p->getId() == productId;
                           });
    if (it == items_.end()) return false;
    items_.erase(it);
    return true;
}

// Расчёты 

double Order::getItemsTotal() const {
    double total = 0.0;
    for (const auto& item : items_) {
        if (!item) continue;

        total += item->getPrice().getValue() * item->getQuantity().getValue();
    }
    return total;
}

double Order::getTotalWeight() const {
    double weight = 0.0;
    for (const auto& item : items_) {
        if (!item) continue;
        weight += item->getWeight().getValue() * item->getQuantity().getValue();
    }
    return weight;
}

double Order::getDeliveryCost() const {
    if (!delivery_ || freeDelivery_) return 0.0;
    return delivery_->getCost(*this);
}

double Order::getTotalPrice() const {
    double items = getItemsTotal();
    double afterDiscount = std::max(0.0, items - discountAmount_);
    return afterDiscount + getDeliveryCost();
}

void Order::addDiscountAmount(double v) {
    double maxDiscount = getItemsTotal();
    discountAmount_ = std::min(discountAmount_ + v, maxDiscount);
}

// Стратегии 

void Order::applyDiscount(const Discount& d)
{
    d.apply(*this);
    discounts_.push_back(d.getName()); // вместо указателя будем ждать имя потмоу что если будет укзаатель моджет быть утечка памяти 
 // можешь почитать про dangling pointer 
}

void Order::setDelivery(std::shared_ptr<Delivery> d) {
    if (d && !d->isAvailable(*this)) // проверка инварианта 
    {
        throw std::invalid_argument("заказ не может иметь недоступную доставку");
    }
    delivery_ = std::move(d);
}

// Статусы и Observer 

void Order::setStatus(OrderStatus s) {
    if (status_ == s) return;
    if (!isTransitionAllowed(status_, s)) {
        throw std::logic_error("Invalid status transition: " +
                               toString(status_) + " -> " + toString(s));
    }
    OrderEvent event(id_, status_, s);
    status_ = s;
    notify(event);
}

void Order::subscribe(IObserver* observer) {
    if (!observer) return;
    if (std::find(observers_.begin(), observers_.end(), observer) == observers_.end()) {
        observers_.push_back(observer);
    }
}

void Order::unsubscribe(IObserver* observer) {
    observers_.erase(std::remove(observers_.begin(), observers_.end(), observer),
                     observers_.end());
}

void Order::notify(const OrderEvent& event) {
    for (IObserver* obs : observers_) {
        if (obs) obs->onOrderEvent(event);
    }
}

//  Перегрузка операторов 

Order Order::operator+(const Discount& d) const {
    Order copy = *this;   // глубокое копирование товаров
    copy.applyDiscount(d);
    return copy;
}
 /// реализация добавленных операторов см в хедере 
Order Order::operator+(const Product& item) const {
    Order copy = *this;
    copy.addItem(std::shared_ptr<Product>(item.clone()));
    return copy;
}

Order Order::operator-(int productId) const {
    Order copy = *this;
    copy.removeItem(productId);
    return copy;
}

Order& Order::operator+=(const Discount& d) {
    applyDiscount(d);
    return *this;
}

void operator<<(const std::shared_ptr<Delivery>& d, Order& order) {
    order.setDelivery(d);
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << "Order#" << order.getId()
       << " [" << order.getCustomer() << ", " << order.getRegion() << "]"
       << " status=" << toString(order.getStatus())
       << " items=" << order.getItems().size()
       << " itemsTotal=" << order.getItemsTotal()
       << " discount=" << order.getDiscountAmount()
       << " cashback=" << order.getCashback()
       << " delivery=" << order.getDeliveryCost()
       << " total=" << order.getTotalPrice();
    if (!order.getDiscounts().empty()) { // журнал применённых промо, видно какие скидки навешаны
        os << " promos=[";
        for (std::size_t i = 0; i < order.getDiscounts().size(); ++i) {
            os << (i ? ", " : "") << order.getDiscounts()[i];
        }
        os << "]";
    }
    return os;
}