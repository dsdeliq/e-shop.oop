#include "WarehouseIterator.h"

WarehouseIterator::WarehouseIterator(Base current, Base end)
    : current_(current), end_(end) {}

// **current_: первая * достаёт unique_ptr из вектора, вторая достаёт сам Product
Product& WarehouseIterator::operator*() const {
    return **current_;
}

// Переход к следующему товару (за конец не выходим)
WarehouseIterator& WarehouseIterator::operator++() {
    if (current_ != end_) {
        ++current_;
    }
    return *this;
}

bool WarehouseIterator::operator!=(const WarehouseIterator& other) const {
    return current_ != other.current_;
}

// ---------- ConstWarehouseIterator ----------
ConstWarehouseIterator::ConstWarehouseIterator(Base current, Base end)
    : current_(current), end_(end) {}

const Product& ConstWarehouseIterator::operator*() const {
    return **current_;
}

ConstWarehouseIterator& ConstWarehouseIterator::operator++() {
    if (current_ != end_) {
        ++current_;
    }
    return *this;
}

bool ConstWarehouseIterator::operator!=(const ConstWarehouseIterator& other) const {
    return current_ != other.current_;
}