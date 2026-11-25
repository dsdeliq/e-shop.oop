#pragma once
#include <chrono>
#include "OrderStatus.h"

using TimePoint = std::chrono::system_clock::time_point;

// Событие смены статуса, которое рассылается наблюдателям
struct OrderEvent {
    int orderId;
    OrderStatus oldStatus;
    OrderStatus newStatus;
    TimePoint   timestamp;

    OrderEvent(int id, OrderStatus from, OrderStatus to)
        : orderId(id), oldStatus(from), newStatus(to),
          timestamp(std::chrono::system_clock::now()) {}
};
