#include "Quantity.h"

Quantity::Quantity(int value): value_(value) {
    if (value<0){
        throw std::invalid_argument("Value cannnot be negative");
    }
}
int Quantity ::getValue() const {
    return value_;
}
void Quantity::setValue(int newValue){
    if(newValue>=0){
        value_=newValue;
    }
    else{
        throw std::invalid_argument("Value cannnot be negative");
    }
}
void Quantity ::change(int delta){
    int newVal = value_ + delta;   // delta — это +10, -3 и т.п.
    if (newVal < 0) throw std::invalid_argument("Quantity can't be negative");
    value_ = newVal;
}