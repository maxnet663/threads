#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

enum Order {
    FREE = 0,
    PIZZA,
    SOUP,
    STEAK,
    SALAD,
    SUSHI
};

std::string interpret_order(Order order) {
    if (order == PIZZA)
        return "Pizza";
    if (order == SOUP)
        return "Soup";
    if (order == STEAK)
        return "Steak";
    if (order == SALAD)
        return "Salad";
    if (order == SUSHI)
        return "Sushi";
    return "Kitchen is free";
}

Order interpret_order(int order) {
    if (order == 0)
        return FREE;
    if (order == 1)
        return PIZZA;
    if (order == 2)
        return SOUP;
    if (order == 3)
        return STEAK;
    if (order == 4)
        return SALAD;
    if (order == 5)
        return SUSHI;
    return FREE;
}

int orders = 0;

std::mutex inWork_access;
Order inWork = FREE;

std::mutex ordersQueue_access;
std::queue<Order> ordersQueue;

std::mutex readyOrders_access;
std::queue<Order> readyOrders;

void new_orders() {
    while (orders < 10) {
        std::srand(std::time(nullptr));
        std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 6 + 5));
        ordersQueue_access.lock();
        ordersQueue.push(interpret_order(std::rand() % 5 + 1));
        std::cout << "New order: " << interpret_order(ordersQueue.back()) << std::endl;
        ordersQueue_access.unlock();
    }
}

void cooking() {
    inWork_access.lock();
    std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 11 + 5));
    std::cout << interpret_order(inWork) << " is ready" << std::endl;
    readyOrders_access.lock();
    readyOrders.push(inWork);
    readyOrders_access.unlock();
    inWork = FREE;
    inWork_access.unlock();
}

void accept_order() {
    while (orders < 10) {
        if (inWork == FREE && !ordersQueue.empty()) {
            ordersQueue_access.lock();
            inWork = ordersQueue.front();
            ordersQueue.pop();
            ordersQueue_access.unlock();
            std::cout << interpret_order(inWork) << " is cooking" << std::endl;
            std::thread cook(cooking);
            cook.detach();
        }
    }
}

void delivery() {
    while (orders < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        readyOrders_access.lock();
        while(!readyOrders.empty()) {
            std::cout << interpret_order(readyOrders.front()) << " has delivered" << std::endl;
            readyOrders.pop();
            orders++;
        }
        readyOrders_access.unlock();
    }
}
    

void kitchen() {

}

int main() {
    std::thread ordersTable(new_orders);
    std::thread orders(accept_order);
    std::thread courier(delivery);

    ordersTable.join();
    orders.join();
    courier.join();
    return 0;
}





















