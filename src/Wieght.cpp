#include "Weight.h"

Weight::Weight(double value): value_(value) {
    if (value<0){
        throw std::invalid_argument("Value cannnot be negative");
    }
}
double Weight::getValue() const {
    return value_;
}
void Weight::setValue(double newValue){
    if(newValue>=0){
        value_=newValue;
    }
    else{
        throw std::invalid_argument("Value cannnot be negative");
    }
}