#include "Warehouse.h"
#include <algorithm>
#include <stdexcept>
#include <utility>   // для std::move

Warehouse::Warehouse( size_t maxSize):maxSize_(maxSize){
    if (maxSize==0){
        throw std:: invalid_argument("Warehouse size must be positive");
    }
}

Warehouse::Warehouse(const Warehouse& other)
    : products_(),maxSize_(other.maxSize_)         // 1. копируем лимиt                       // 2. начинаем с пустого вектора
{
    products_.reserve(other.products_.size());   // 3. резервируем место

    for (const auto& product : other.products_) { // 4. идём по товарам "оригинала"
        auto copy = product->clone();
        copy->setOwner(this);
        products_.push_back(std::move(copy));
    }
}

Warehouse& Warehouse::operator=(const Warehouse& other) {
    // 1. Проверка на самоприсваивание
    if (this == &other) {
        return *this;   // присваиваем себе себя — ничего не делаем
    }

    // 2. Копируем "размер шкафа"
    maxSize_ = other.maxSize_;

    // 3. Выбрасываем старые вещи
    products_.clear();

    // 4. Резервируем место
    products_.reserve(other.products_.size());

    // 5. Копируем вещи через clone()
    for (const auto& product : other.products_) {
        auto copy = product->clone();
        copy->setOwner(this);
        products_.push_back(std::move(copy));
    }

    // 6. Возвращаем себя
    return *this;
}

// Конструктор перемещения
Warehouse::Warehouse(Warehouse&& other) noexcept
    : products_(std::move(other.products_)),   // перекладываем владение
      maxSize_(other.maxSize_)
{
    for (auto& product : products_) {
        product->setOwner(this);
    }

    // оставляем источник в валидном пустом состоянии
    other.maxSize_ = 0;
    // other.products_ уже пуст после std::move
}
Warehouse& Warehouse::operator=(Warehouse&& other) noexcept { //присваниеперемещ
    if (this != &other) {
        products_ = std::move(other.products_);  // забираем вектор unique_ptr
        maxSize_  = other.maxSize_;              // копируем число (это просто size_t)
        for (auto& product : products_) {
            product->setOwner(this);
        }
        other.maxSize_ = 0;                      // «обнуляем» источник (по желанию)
    }
    return *this;
}

void Warehouse::addProduct(std::unique_ptr<Product> product) {
    if (products_.size() >= maxSize_) {
        throw std::overflow_error("Warehouse is full");
    }
    product->setOwner(this);
    products_.push_back(std::move(product));
}

bool Warehouse::removeById(int id) {
    auto it = std::find_if(products_.begin(), products_.end(),
        [id](const auto& p) { return p->getId() == id; });
    if (it == products_.end()) return false;
    (*it)->setOwner(nullptr);   // товар больше не на складе
    products_.erase(it);       // unique_ptr сам удалит объект
    return true;
}

bool Warehouse::removeByIndex(size_t index) {
    if (index >= products_.size()) return false;
    products_[index]->setOwner(nullptr);
    products_.erase(products_.begin() + index);
    return true;
}

std::unique_ptr<Product> Warehouse::extractById(int id) {
    auto it = std::find_if(products_.begin(), products_.end(),
        [id](const auto& product) {
            return product->getId() == id;
        });

    if (it == products_.end()) {
        return nullptr;
    }

    auto product = std::move(*it);
    products_.erase(it);
    product->setOwner(nullptr);
    return product;
}

Product* Warehouse::findById(int id) {
    for (auto& product : products_) {
        if (product->getId() == id) {
            return product.get();   // .get() достаёт сырой указатель из unique_ptr
        }
    }
    return nullptr;   // не нашли — честно сообщаем об этом
}

const Product* Warehouse::findById(int id) const {
    for (const auto& product : products_) {
        if (product->getId() == id) {
            return product.get();
        }
    }
    return nullptr;
}

std::vector<Product*>Warehouse::findByCategory(const std::string& category) const{
    std::vector<Product*> result;

    for(const auto& product : products_){
        if(product->getCategory()== category){
            result.push_back(product.get());
        }
    }
    return result;

}
std::vector<Product*>Warehouse::findByPrice(const Price &price) const{
     std::vector<Product*> result;

     for(const auto& product : products_){
        if (product->getPrice().getValue() == price.getValue()){
            result.push_back(product.get());
        } 
     }
     return result;
}

std::size_t Warehouse::size() const {
    return products_.size();
}

std::size_t Warehouse::maxSize() const {
    return maxSize_;
}
// Итерация
WarehouseIterator Warehouse::begin() {
    return WarehouseIterator(products_.begin(), products_.end());
}
 
WarehouseIterator Warehouse::end() {
    return WarehouseIterator(products_.end(), products_.end());
}
 
ConstWarehouseIterator Warehouse::begin() const {
    return ConstWarehouseIterator(products_.begin(), products_.end());
}
 
ConstWarehouseIterator Warehouse::end() const {
    return ConstWarehouseIterator(products_.end(), products_.end());
}
