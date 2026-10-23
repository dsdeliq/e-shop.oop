#include <iostream>
#include <memory>
#include "Warehouse.h"
#include "ProductFactory.h"

void scenario1() {
    std::cout << "Сценарий 1создание склада и обход------\n";

    // создаём склад с лимитом, например, на 10 товаров
    Warehouse warehouse(10);

    // создаём фабрики
    SmartphoneFactory phoneFactory;
    LaptopFactory     laptopFactory;
    TShirtFactory     tshirtFactory;

    // наполняем склад через фабрики
    warehouse.addProduct(phoneFactory.createProduct(1, "iPhone 15", 999, 5, 0.2));
    warehouse.addProduct(phoneFactory.createProduct(2, "Galaxy S24", 899, 3, 0.18));
    warehouse.addProduct(laptopFactory.createProduct(3, "MacBook Pro", 2499, 2, 1.6));
    warehouse.addProduct(tshirtFactory.createProduct(4, "Basic Tee", 25, 20, 0.15));
    

    // it ссылка на товар /обходим через итератор
    std::cout << "\nТовары на складе:\n";
    for (auto it = warehouse.begin(); it != warehouse.end(); ++it) {
        // зависит от того, что возвращает operator*
        std::cout << (*it).getName()
                  << " | price=" << (*it).getPrice().getValue()
                  << " | qty="   << (*it).getQuantity().getValue()
                  << "\n\n";
    }
}
 void scenario2(){//копирование изм в копии проверка независим
    std::cout<<"Сценарий 2------\n";

    Warehouse original(10);
    MilkFactory milkfactory;
    original.addProduct(milkfactory.createProduct(5, "Milk 1L", 6, 50, 1.0));
    original.addProduct(milkfactory.createProduct(6, "Milk 2L", 5, 60, 1.0));
    
    Warehouse copy= original;
    Product* prdct=copy.findById(5);
    if (prdct) {
        prdct->setPrice(Price(500));       // цена в копии стала 500
        prdct->changeQuantity(-2);          // остаток уменьшился на 2
    }

    // проверяем оригинал
    Product* orig = original.findById(5);
    std::cout << "Оригинал: цена = " << orig->getPrice().getValue()
              << ", колво-остаток = " << orig->getQuantity().getValue() << "\n";
    std::cout << "Копия:    цена = " << prdct->getPrice().getValue()
              << ", колво-остаток = " << prdct->getQuantity().getValue() << "\n\n";

 }
 void scenario3(){// перемещение склада через констр перемещения 
    std::cout<<"Сценарий 3-------\n";

    // создаём исходный склад
    Warehouse source(5);
    SmartphoneFactory phoneFactory;
    LaptopFactory     laptopFactory;

    source.addProduct(phoneFactory.createProduct(1, "iPhone 15", 999, 5, 0.2));
    source.addProduct(phoneFactory.createProduct(2, "Galaxy S24", 899, 3, 0.18));
    source.addProduct(laptopFactory.createProduct(3, "MacBook Pro", 2499, 2, 1.6));

    std::cout << "Товаров в source до перемещения: " << source.size() << "\n";

    // перемещаем склад через конструктор перемещения
    Warehouse moved = std::move(source);

    // проверяем результат
    std::cout << "Товаров в moved после перемещения: " << moved.size() << "\n";
    std::cout << "Товаров в source после перемещения: " << source.size() << "\n";

    // обходим moved через итератор — товары на месте
    std::cout << "\nТовары в moved:\n";
    for (auto it = moved.begin(); it != moved.end(); ++it) {
        std::cout << "  " << (*it).getName()
                  << " | price=" << (*it).getPrice().getValue() << "\n\n";
    }
 }

 void scenario4() {
    std::cout << "Сценарий 4: поиск и удаление------\n";

    Warehouse warehouse(10);
    SmartphoneFactory phoneFactory;
    LaptopFactory     laptopFactory;
    TShirtFactory     tshirtFactory;
    MilkFactory       milkFactory;

    warehouse.addProduct(phoneFactory.createProduct(1, "iPhone 15", 999, 5, 0.2));
    warehouse.addProduct(phoneFactory.createProduct(2, "Galaxy S24", 899, 3, 0.18));
    warehouse.addProduct(laptopFactory.createProduct(3, "MacBook Pro", 2499, 2, 1.6));
    warehouse.addProduct(tshirtFactory.createProduct(4, "Basic Tee", 25, 20, 0.15));
    warehouse.addProduct(milkFactory.createProduct(5, "Milk 1L", 2, 50, 1.0));

    std::cout << "Товаров на складе: " << warehouse.size() << "\n";

    // поиск по ID
    std::cout << "\n--- Поиск по ID = 3 ---\n";
    if (Product* p = warehouse.findById(3)) {
        std::cout << "Найден: " << p->getName()
                  << ", цена: " << p->getPrice().getValue() << "\n";
    } else {
        std::cout << "Товар с таким ID не найден\n";
    }

    // поиск по категории
    std::cout << "\n--- Поиск по категории 'electronics' ---\n";
    auto electronics = warehouse.findByCategory("electronics");
    for (Product* p : electronics) {
        std::cout << p->getName() << "\n";
    }

    // поиск по цене
    std::cout << "\n--- Поиск по цене = 999 ---\n";
    auto byPrice = warehouse.findByPrice(Price(999));
    for (Product* p : byPrice) {
        std::cout << p->getName() << "\n";
    }

    // удаление по ID
    std::cout << "\n--- Удаление по ID = 2 ---\n";
    warehouse.removeById(2);
    std::cout << "Товаров после удаления: " << warehouse.size() << "\n";

    // удаление по индексу
    std::cout << "\n--- Удаление по индексу 0 ---\n";
    warehouse.removeByIndex(0);
    std::cout << "Товаров после удаления: " << warehouse.size() << "\n";

    //что осталось
    std::cout << "\nОставшиеся товары:\n";
    for (auto it = warehouse.begin(); it != warehouse.end(); ++it) {
        std::cout << (*it).getName() << "\n";
    }
}

void scenario5() {
    std::cout << "Сценарий 5 перенос товара-----\n";

    Warehouse warehouseA(10);
    Warehouse warehouseB(10);
    SmartphoneFactory phoneFactory;

    //создаём товар и запоминаем ID который Product выдал сам
    auto phone = phoneFactory.createProduct(1, "iPhone 15", 999, 5, 0.2);
    const int id = phone->getId();

    warehouseA.addProduct(std::move(phone));//передаю владния на А

    std::cout << "До переноса: A = " << warehouseA.size()
              << ", B = " << warehouseB.size() << "\n";

    // берём товар из A и переносим в B
    if (Product* product = warehouseA.findById(id)) {
        product->transferTo(warehouseB);
    }

    std::cout << "После переноса: A = " << warehouseA.size()
              << ", B = " << warehouseB.size() << "\n";

    // товар теперь находится в B — изменяем его там
    if (Product* product = warehouseB.findById(id)) {
        product->changeQuantity(-2);
        product->setPrice(Price(799));

        std::cout << product->getName()
                  << ": цена = " << product->getPrice().getValue()
                  << ", остаток = " << product->getQuantity().getValue()
                  << "\n";
    }

    if (warehouseA.findById(id) == nullptr) {
        std::cout << "Товара в A больше нет\n";
    }
}
int main(){
    scenario1();
    scenario2();
    scenario3();
    scenario4();
    scenario5();
    return 0;
}