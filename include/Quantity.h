#pragma once
#include <stdexcept>

// Обёртка над значением с валидацией (значение не может быть отрицательным)
class Quantity {
private:
    int value_;
public:
    Quantity(int value);                       // бросает std::invalid_argument при value < 0
    int getValue() const;
    void setValue(int newValue);        // бросает std::invalid_argument при newValue < 0
    void change(int delta);

};
