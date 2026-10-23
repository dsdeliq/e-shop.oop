#include "ProductFactory.h"
#include "Electronics.h"
#include "Clothing.h"
#include "Food.h"
// Параметр id не используется: уникальный id товару выдаёт Product (счётчик nextId_)

std::unique_ptr<Product> SmartphoneFactory::createProduct(int, const std::string& name,
                                                          int price, int quantity,
                                                          double weight) {
    return std::make_unique<Smartphone>(name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> LaptopFactory::createProduct(int, const std::string& name,
                                                      int price, int quantity,
                                                      double weight) {
    return std::make_unique<Laptop>(name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> TShirtFactory::createProduct(int, const std::string& name,
                                                      int price, int quantity,
                                                      double weight) {
    return std::make_unique<TShirt>(name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> JacketFactory::createProduct(int, const std::string& name,
                                                      int price, int quantity,
                                                      double weight) {
    return std::make_unique<Jacket>(name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> MilkFactory::createProduct(int, const std::string& name,
                                                    int price, int quantity,
                                                    double weight) {
    return std::make_unique<Milk>(name, Price(price), Quantity(quantity), Weight(weight));
}

std::unique_ptr<Product> BreadFactory::createProduct(int, const std::string& name,
                                                     int price, int quantity,
                                                     double weight) {
    return std::make_unique<Bread>(name, Price(price), Quantity(quantity), Weight(weight));
}

// #include <memory>
// #include <string>
// #include "ProductFactory.h"
// #include "Electronics.h"
// #include "Clothing.h"
// #include "Food.h"

// class ProductFactory {
// public:
//     virtual ~ProductFactory() = default;
//     virtual std::unique_ptr<Product> createProduct(int id, const std::string& name,
//                                                    int price, int quantity,
//                                                    double weight) = 0;
// };

// class SmartphoneFactory : public ProductFactory {
// public:
//     std::unique_ptr<Product> createProduct(int id, const std::string& name,
//                                            int price, int quantity,
//                                            double weight) override;
// };

// class LaptopFactory : public ProductFactory {
// public:
//     std::unique_ptr<Product> createProduct(int id, const std::string& name,
//                                            int price, int quantity,
//                                            double weight) override;
// };

// class TShirtFactory : public ProductFactory {
// public:
//     std::unique_ptr<Product> createProduct(int id, const std::string& name,
//                                            int price, int quantity,
//                                            double weight) override;
// };

// class JacketFactory : public ProductFactory {
// public:
//     std::unique_ptr<Product> createProduct(int id, const std::string& name,
//                                            int price, int quantity,
//                                            double weight) override;
// };

// class MilkFactory : public ProductFactory {
// public:
//     std::unique_ptr<Product> createProduct(int id, const std::string& name,
//                                            int price, int quantity,
//                                            double weight) override;
// };

// class BreadFactory : public ProductFactory {
// public:
//     std::unique_ptr<Product> createProduct(int id, const std::string& name,
//                                            int price, int quantity,
//                                            double weight) override;
// };
