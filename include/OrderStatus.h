#pragma once
#include <ostream>
#include <string>

enum class OrderStatus { Pending, Paid, Shipped, Delivered, Cancelled };

inline std::string toString(OrderStatus s) {
    switch (s) {
        case OrderStatus::Pending:   return "pending";
        case OrderStatus::Paid:      return "paid";
        case OrderStatus::Shipped:   return "shipped";
        case OrderStatus::Delivered: return "delivered";
        case OrderStatus::Cancelled: return "cancelled";
    }
    return "unknown";
}

// Допустимые переходы: pending->paid|cancelled, paid->shipped|cancelled, shipped->delivered
inline bool isTransitionAllowed(OrderStatus from, OrderStatus to) {
    switch (from) {
        case OrderStatus::Pending: return to == OrderStatus::Paid || to == OrderStatus::Cancelled;
        case OrderStatus::Paid:    return to == OrderStatus::Shipped || to == OrderStatus::Cancelled;
        case OrderStatus::Shipped: return to == OrderStatus::Delivered;
        default: return false;
    }
}

// «Активный» заказ - ещё не завершён и не отменён
inline bool isActive(OrderStatus s) {
    return s == OrderStatus::Pending || s == OrderStatus::Paid || s == OrderStatus::Shipped;
}

inline std::ostream& operator<<(std::ostream& os, OrderStatus s) { return os << toString(s); }
