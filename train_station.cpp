#include <thread>
#include <iostream>
#include <mutex>
#include <string>

struct Train {
    int num = 0;
    int travelTime = 0;

    void fill (Train *trains, int n) {
        for (int i = 0; i < n; i++) {
            trains[i].num = i;
            std::cout << "Enter the arriving time:" << std::endl;
            std::cin >> trains[i].travelTime;
        }
    }
};

std::mutex railwayStation_access;
bool railwayStation = 0; // 0 means station is empty, 1 - has train

void arriving(Train *train) {
    railwayStation = 1;
    std::cout << "Train number " << train->num << " arrived on the station" << std::endl;
    std::string depart;
    while (depart != "depart")
        std::cin >> depart;
    std::cout << "Train number " << train->num << " is leaving from the station" << std::endl;
    railwayStation = 0;
}

void on_the_way(Train *train) {
    std::this_thread::sleep_for(std::chrono::seconds(train->travelTime));
    if (railwayStation)
        std::cout << "Train number " << train->num << " is waiting" << std::endl;
    railwayStation_access.lock();
    arriving(train);
    railwayStation_access.unlock();
}

int main() {
    Train trains[3];
    trains->fill(trains, 3);

    std::thread train0(on_the_way, &trains[0]);
    std::thread train1(on_the_way, &trains[1]);
    std::thread train2(on_the_way, &trains[2]);

    train0.join();
    train1.join();
    train2.join();
    
    return 0;
}
