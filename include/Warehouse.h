#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "Price.h"
#include "Product.h"
#include "WarehouseIterator.h"

class Warehouse {
    std::vector<std::unique_ptr<Product>> products_;
    std::size_t maxSize_;    
public:
    explicit Warehouse(std::size_t maxSize);

    // Правило пяти
    ~Warehouse()=default;
    Warehouse(const Warehouse& other);                   // глубокая копия через clone()
    Warehouse& operator=(const Warehouse& other);        // copy-and-swap или поэлементно
    Warehouse(Warehouse&& other) noexcept;
    Warehouse& operator=(Warehouse&& other) noexcept;

    // Добавление / удаление
    void addProduct(std::unique_ptr<Product> product);   // бросает исключение при превышении maxSize_
    bool removeById(int id);
    bool removeByIndex(std::size_t index);
    std::unique_ptr<Product> extractById(int id);        // вынуть товар без удаления (для transferTo)

    // Поиск
    const Product* findById(int id) const; 
    Product* findById(int id);                    // nullptr, если не найден
    std::vector<Product*> findByCategory(const std::string& category) const;
    std::vector<Product*> findByPrice(const Price& price) const;

    // Информация
    std::size_t size() const;
    std::size_t maxSize() const;

    // Итерация
    WarehouseIterator begin();
    WarehouseIterator end();
    ConstWarehouseIterator begin() const;
    ConstWarehouseIterator end() const;
 


};
