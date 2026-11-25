#include "Delivery.h"
#include "Order.h"
double CourierDelivery::getCost(const Order&) const { return 500.0; }
std::string CourierDelivery::getEstimatedTime() const { return "1-2 days"; }
bool CourierDelivery::isAvailable(const Order& order) const { return order.getTotalWeight() <= 20.0; }

double PickupDelivery::getCost(const Order&) const { return 0.0; }
std::string PickupDelivery::getEstimatedTime() const { return "1-3 days"; }
bool PickupDelivery::isAvailable(const Order&) const { return true; }

double PostDelivery::getCost(const Order&) const { return 200.0; }
std::string PostDelivery::getEstimatedTime() const { return "5-10 days"; }
bool PostDelivery::isAvailable(const Order& order) const { return order.getTotalWeight() <= 10.0; }
