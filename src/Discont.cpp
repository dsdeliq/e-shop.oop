#include "Discount.h"
#include "Order.h"
#include "Product.h"
#include <sstream>
#include <stdexcept>

namespace 
{
void checkPercent(double p) // нельзя создать скидку −10 или 150 защищаем инвариант
{
    if (p < 0.0 || p > 100.0) 
    {
        throw std::invalid_argument("процент должен находиться в диапазоне от 0 до 100"); // кидаем исклчение 
    } 
}
std::string fmt(double v) // переводим даьл в стринг 
{
    std::ostringstream os;
    os << v;
    return os.str();
}
}
 
// PercentDiscount 
PercentDiscount::PercentDiscount(double percent) : percent_(percent) { checkPercent(percent); }

void PercentDiscount::apply(Order& order) const {
    order.addDiscountAmount(order.getItemsTotal() * (percent_ / 100.0));
}

std::string PercentDiscount::getName() const { return "PercentDiscount(" + fmt(percent_) + "%)"; }

//CashbackDiscount 
CashbackDiscount::CashbackDiscount(double percent) : percent_(percent) { checkPercent(percent); }

void CashbackDiscount::apply(Order& order) const {
    order.addCashback(order.getItemsTotal() * (percent_ / 100.0));
}

std::string CashbackDiscount::getName() const { return "Cashback(" + fmt(percent_) + "%)"; }

// FreeDeliveryDiscount 
void FreeDeliveryDiscount::apply(Order& order) const {
    order.setFreeDelivery(true);
}

// GiftDiscount 
GiftDiscount::GiftDiscount(std::shared_ptr<Product> gift) : gift_(std::move(gift)) 
{ // валидация ненулевого прототипа
    if (!gift_) throw std::invalid_argument("Gift product must not be null");
}

void GiftDiscount::apply(Order& order) const 
{
    
    std::unique_ptr<Product> gift = gift_->clone();
    gift->setPrice(Price(0));
    gift->changeQuantity(1 - gift->getQuantity().getValue());
    order.addItem(std::shared_ptr<Product>(std::move(gift)));
}

std::string GiftDiscount::getName() const { return "Gift(" + gift_->getName() + ")"; }

