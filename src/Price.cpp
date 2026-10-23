#include "Price.h"

Price::Price(int value) : value_(value) {
    if (value<0){
        throw std::invalid_argument("Value cannnot be negative");
    }
}
int Price::getValue() const {
    return value_;
}
void Price::setValue(int newValue){
    if(newValue>=0){
        value_=newValue;

    }
}
