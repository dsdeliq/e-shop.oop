#include <iostream>
#include <memory>
#include "Warehouse.h"
#include "ProductFactory.h"
#include "Discount.h"
#include "ProductType.h"
#include "OrderManager.h"
#include "Delivery.h"
#include "DeliveryProxy.h"
#include <string>
#include <unordered_set>
#include <vector>
#ifdef _WIN32
#endif

void lab1_scenario1() {
    std::cout << "Лаба 1, сценарий 1: создание склада и обход\n";

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
 void lab1_scenario2(){//копирование изм в копии проверка независим
    std::cout<<"Лаба 1, сценарий 2: копирование склада\n";

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
 void lab1_scenario3(){// перемещение склада через констр перемещения 
    std::cout<<"Лаба 1, сценарий 3: перемещение склада\n";

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

 void lab1_scenario4() {
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

void lab1_scenario5() {
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


// ЛАБА2 сценарий 1 создание заказа через OrderManager, добавление товаров,
// применение процентной скидки через order + discount
void lab2_scenario1() {
    std::cout<<"Лаба 2, сценарий 1: заказ и процентная скидка"<<"\n\n";

    OrderManager manager;
    SmartphoneFactory phoneFactory;
    MilkFactory milkFactory;

    // заказ создается через менеджер без товаров
    const int orderId = manager.createOrder("Анна", {}, "Москва");
    Order* order = manager.getOrder(orderId);
    if (!order) return;

    // добавляем товары
    order->addItem(phoneFactory.createProduct(1, "iPhone 15", 999, 1, 0.2));
    order->addItem(milkFactory.createProduct(2, "Milk 1L", 100, 2, 1.0));
    std::cout << "До скидки:  " << *order << '\n';

    // 3. order + discount -> НОВЫЙ заказ, исходный не меняется
    PercentDiscount discount(10.0);
    Order discounted = *order + discount;
    std::cout << "order + discount:   " << discounted << '\n';
    std::cout << "Исходный заказ:     " << *order << "   (не изменился)\n";

    // 4. чтобы скидку получил заказ внутри менеджера, меняем его самого
    *order += discount;
    std::cout << "order += discount: " << *order << '\n';
    std::cout << "Заказ в менеджере теперь стоит " << manager.getOrder(orderId)->getTotalPrice() << '\n';
}
//получили копию заказа со скидкой, не меняя оригинал
//применить скидку непосредственно к исходному заказу. есть копия исходного заказа тут и стратегия

// сценарий 2: выбор способа доставки через delivery  order, проверка через DeliveryProxy
void lab2_scenario2() {
    std::cout<<"Лаба 2, сценарий 2: доставка и DeliveryProxy"<<"\n\n";

    OrderManager manager;
    SmartphoneFactory phoneFactory;
    LaptopFactory laptopFactory;

    // Курьер через прокси: обслуживает только Москву и Спб, запасной способ - самовывоз
    const std::unordered_set<std::string> regions{"Москва", "Спб"};
    std::shared_ptr<Delivery> courier = std::make_shared<DeliveryProxy>(
        std::make_shared<CourierDelivery>(), regions, std::make_shared<PickupDelivery>());

    // обычный заказ 
    int id1 = manager.createOrder("Борис", {phoneFactory.createProduct(0, "iPhone 15", 999, 1, 0.2)}, "Москва");
    Order& light = *manager.getOrder(id1);

    courier << light;                                   // delivery << order
    std::cout << "Время доставки: " << light.getDelivery()->getEstimatedTime() << '\n';
    const double firstCost = light.getDeliveryCost();    // первый вызов считает и кеширует
    std::cout << "Стоимость доставки: " << firstCost << '\n';
    const double cachedCost = light.getDeliveryCost();   // второй берёт из кеша (лога нет)
    std::cout << "Повторный запрос (из кеша прокси): " << cachedCost << '\n';
    std::cout << light << '\n';

    // --- регион не обслуживается
    std::cout << "\nРегион вне зоны доставки:\n";
    int id2 = manager.createOrder("Вера", {phoneFactory.createProduct(0, "Galaxy S24", 899, 1, 0.18)}, "Владивосток");
    try {
        courier << *manager.getOrder(id2);
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }

    // --- тяжёлый заказ 25 кг курьер до 20 кг
    std::cout << "\nТяжёлый заказ (25 кг):\n";
    int id3 = manager.createOrder("Глеб", {laptopFactory.createProduct(0, "Workstation", 3000, 1, 25.0)}, "Москва");
    Order& heavy = *manager.getOrder(id3);

    std::shared_ptr<Delivery> post = std::make_shared<PostDelivery>();
    try {
        post << heavy;
    } catch (const std::exception& e) {
        std::cout << "Почта: " << e.what() << '\n';
    }
    // прокси с запасным способом сам переключится на самовывоз
    courier << heavy;
    const double heavyCost = heavy.getDeliveryCost();
    std::cout << "Через прокси назначено, стоимость доставки: " << heavyCost << '\n';
    std::cout << heavy << "\n\n";
}

// Сценарий 3: смена статуса, уведомление OrderManager (Observer), обновление оборота
void lab2_scenario3() {
    std::cout<<"Лаба 2, сценарий 3: статусы и Observer";

    OrderManager manager;
    SmartphoneFactory phoneFactory;

    int id = manager.createOrder("Дарья", {phoneFactory.createProduct(0, "iPhone 15", 999, 1, 0.2)}, "Москва");
    std::shared_ptr<Delivery> courier = std::make_shared<CourierDelivery>();
    courier << *manager.getOrder(id);
    std::cout << *manager.getOrder(id) << "\nОборот: " << manager.getTurnover() << "\n\n";

    manager.updateStatus(id, OrderStatus::Paid);       // оборот растёт на 999 + 500
    manager.updateStatus(id, OrderStatus::Shipped);

    std::cout << "\nПопытка вернуть Shipped -> Pending: "
              << (manager.updateStatus(id, OrderStatus::Pending) ? "выполнено" : "отклонено") << '\n';
    std::cout << "Попытка изменить несуществующий заказ: "
              << (manager.updateStatus(999, OrderStatus::Paid) ? "выполнено" : "отклонено") << "\n\n";

    manager.updateStatus(id, OrderStatus::Delivered);
    std::cout << "\nИтоговый оборот: " << manager.getTurnover()
              << ", клиентов: " << manager.getCustomerCount() << '\n';

    // отписка: после unsubscribe менеджер больше не получает события заказа
    Order standalone(100, "Тест");
    standalone.subscribe(&manager);
    standalone.unsubscribe(&manager);
    standalone.setStatus(OrderStatus::Paid);            // тишина: менеджер отписан
    std::cout << "Смена статуса после unsubscribe прошла без уведомления менеджера\n";
}

// Сценарий 4: несколько скидок к одному заказу, проверка итоговой цены
void lab2_scenario4() {
    std::cout<<"Лаба 2, сценарий 4: несколько скидок";

    OrderManager manager;
    SmartphoneFactory phoneFactory;
    TShirtFactory tshirtFactory;

    int id = manager.createOrder("Егор", {
        phoneFactory.createProduct(0, "iPhone 15", 999, 1, 0.2),
        tshirtFactory.createProduct(0, "Basic Tee", 25, 2, 0.15)}, "Москва");
    Order& order = *manager.getOrder(id);

    std::shared_ptr<Delivery> courier = std::make_shared<CourierDelivery>();
    courier << order;
    std::cout << "Без акций:        " << order << "\n";           // 1049 + 500 = 1549

    PercentDiscount percent(10);
    CashbackDiscount cashback(5);
    FreeDeliveryDiscount freeDelivery;
    GiftDiscount gift(tshirtFactory.createProduct(0, "Подарочная футболка", 25, 1, 0.15));

    order += percent;
    std::cout << "+ 10% скидка:     " << order << "\n";           // -104.9
    order += cashback;
    std::cout << "+ кешбэк 5%:      " << order << "\n";           // цену не меняет, +52.45 бонусов
    order += freeDelivery;
    std::cout << "+ бесплатная дост: " << order << "\n";          // доставка 0
    order += gift;
    std::cout << "+ подарок:        " << order << "\n";           // товаров +1, цена та же

    std::cout << "\nОжидаемо: 1049 - 104.9 = 944.1; кешбэк 52.45; доставка 0; подарок бесплатный\n";
    std::cout << "Итого к оплате: " << order.getTotalPrice()
              << ", бонусов: " << order.getCashback()
              << ", позиций в заказе: " << order.getItems().size() << '\n';
}

// сценарий 5: отмена заказа, освобождение лимита активных заказов, статистика клиентов
void lab2_scenario5() {
    std::cout<<"Лаба 2, сценарий 5: отмена и лимиты";

    OrderManager manager(2);   // не более 2 активных заказов на клиента
    MilkFactory milkFactory;
    auto item = [&] { return std::vector<std::shared_ptr<Product>>{milkFactory.createProduct(0, "Milk 1L", 100, 1, 1.0)}; };

    int a1 = manager.createOrder("Жанна", item());
    manager.createOrder("Жанна", item());
    manager.createOrder("Захар", item());
    std::cout << "Активных у Жанны: " << manager.getActiveOrderCount("Жанна")
              << ", клиентов: " << manager.getCustomerCount() << "\n";

    try {
        manager.createOrder("Жанна", item());
    } catch (const std::exception& e) {
        std::cout << "Третий заказ Жанны: " << e.what() << '\n';
    }

    std::cout << "\nОтменяем заказ #" << a1 << ":\n";
    manager.updateStatus(a1, OrderStatus::Cancelled);
    std::cout << "Активных у Жанны: " << manager.getActiveOrderCount("Жанна") << "\n";

    manager.createOrder("Жанна", item());
    std::cout << "Новый заказ Жанны создан. Активных всего: " << manager.getActiveOrderCount()
              << ", уникальных клиентов: " << manager.getCustomerCount() << "\n";

    // отмена оплаченного заказа возвращает деньги: оборот уменьшается
    int paidId = manager.createOrder("Захар", item());
    manager.updateStatus(paidId, OrderStatus::Paid);
    std::cout << "Оборот после оплаты: " << manager.getTurnover() << '\n';
    manager.updateStatus(paidId, OrderStatus::Cancelled);
    std::cout << "Оборот после отмены оплаченного: " << manager.getTurnover() << '\n';
}

// // дополнительно: операторы Order (+, -, +=, <<), глубокое копирование, легковес
// void lab2_operators() {
//     std::cout<<"Лаба 2, операторы Order, копирование, легковес";

//     SmartphoneFactory phoneFactory;
//     LaptopFactory laptopFactory;
//     std::shared_ptr<Product> phone = phoneFactory.createProduct(0, "iPhone 15", 999, 1, 0.2);
//     const int phoneId = phone->getId();

//     Order base(1, "Игорь");
//     base.addItem(phone);
//     std::cout << "base:                 " << base << '\n';

//     auto laptop = laptopFactory.createProduct(0, "MacBook Pro", 2499, 1, 1.6);
//     Order withLaptop = base + *laptop;                 // order + product
//     std::cout << "base + laptop:        " << withLaptop << '\n';

//     Order withoutPhone = withLaptop - phoneId;         // order - productId
//     std::cout << "(base + laptop) - phone: " << withoutPhone << '\n';
//     std::cout << "base без изменений:   " << base << '\n';

//     // глубокое копирование: изменение товара в копии не затрагивает оригинал
//     Order copy = base;
//     copy.getItems()[0]->setPrice(Price(1));
//     std::cout << "Цена в копии: " << copy.getItems()[0]->getPrice().getValue()
//               << ", в оригинале: " << base.getItems()[0]->getPrice().getValue() << '\n';

//     // перемещение
//     Order moved = std::move(copy);
//     std::cout << "После move в moved " << moved.getItems().size() << " товар(ов)\n";

//     // легковес: товары одной категории делят один ProductType
//     std::cout << "Легковес: тип iPhone и MacBook один и тот же объект? "
//               << (&phone->getType() == &laptop->getType() ? "да" : "нет")
//               << ", типов в пуле:" << ProductType::poolSize() << '\n';
// }

int main() {
#ifdef _WIN32
      
#endif
    // lab1_scenario1();
    // lab1_scenario2();
    // lab1_scenario3();
    // lab1_scenario4();
    // lab1_scenario5();

    lab2_scenario1();
    lab2_scenario2();
    lab2_scenario3();
    lab2_scenario4();
    lab2_scenario5();
    // lab2_operators();
    return 0;
}
