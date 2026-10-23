#pragma once
#include <stdexcept>

// Обёртка над значением с валидацией (значение не может быть отрицательным)
class Weight {
public:
    Weight(double value);                       // бросает std::invalid_argument при value < 0
    double getValue() const;
    void setValue(double newValue);         // бросает std::invalid_argument при newValue < 0

private:
    double value_;
};
