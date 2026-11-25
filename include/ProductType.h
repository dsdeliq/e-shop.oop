#pragma once
#include <memory>
#include <string>
#include <unordered_map>

// Класс-легковес: общие данные категории товара
class ProductType {
    std::string category_;
    double baseWeight_;
    double basePrice_;

    // статический пул легковесов 
    static std::unordered_map<std::string, std::shared_ptr<ProductType>> pool_; // указатель типа 
    //  удобно отдавать наружу

public:
    ProductType(const std::string& category, double baseWeight, double basePrice);

    // базовые веси цена задаются внутри пула по категории
    static std::shared_ptr<const ProductType> getType(const std::string& category);

    // смотрит скольок ращных типов в пуле чисто чтоб показать легковес как раотает 
    static std::size_t poolSize();

    const std::string& getCategory() const;
    double getBaseWeight() const;
    double getBasePrice() const;
};
