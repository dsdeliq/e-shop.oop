#pragma once
#include <stdexcept>

// Обёртка над значением с валидацией (значение не может быть отрицательным)
class Price {
private:
    int value_;
public:
    Price(int value);                       // бросает std::invalid_argument при value < 0
    int getValue() const;
    void setValue(int newValue);         // бросает std::invalid_argument при newValue < 0
    bool operator==(const Price& other) const;
};