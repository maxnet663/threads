#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

struct Swimmer {
    std::string name = "";
    int speed = 0;

    void fill(Swimmer *s, int n) {
        for (int i = 0; i < n; i++) {
            std::cout << "Enter the name of the swimmer:" << std::endl;
            std::cin >> s[i].name;
            std::cout << "Enter the speed of the swimmer:" << std::endl;
            std::cin >> s[i].speed;
        }
    }
};

std::mutex cout_access;
std::mutex scoreboard_access;
std::vector<std::pair<std::time_t, Swimmer>> scoreboard;

void put_line() {
    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout_access.lock();
        std::cout << std::endl << "-------------------------------" << std::endl;
        cout_access.unlock();
    }
}
void swimm(Swimmer *s){
    std::time_t start = std::time(0); 
    int distance = 0;
    while (distance < 100) {
        distance += s->speed;    
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout_access.lock();
        std::cout << s->name << ": " << distance << std::endl;
        cout_access.unlock();
    }
    scoreboard_access.lock();
    scoreboard.push_back(std::pair<int, Swimmer>(std::difftime(std::time(0), start), *s));
    scoreboard_access.unlock();
}

int main() {
   Swimmer swimmers[6]; 
   swimmers->fill(swimmers, 6);
   std::thread lines(put_line);
   std::thread swimmer0(swimm, &swimmers[0]);
   std::thread swimmer1(swimm, &swimmers[1]);
   std::thread swimmer2(swimm, &swimmers[2]);
   std::thread swimmer3(swimm, &swimmers[3]);
   std::thread swimmer4(swimm, &swimmers[4]);
   std::thread swimmer5(swimm, &swimmers[5]);

   swimmer0.join();
   swimmer1.join();
   swimmer2.join();
   swimmer3.join();
   swimmer4.join();
   swimmer5.join();

   for (int i = 0; i < scoreboard.size(); i++) {
       std::cout << "#" << i + 1 << " " << scoreboard[i].second.name << " Time: " 
           << std::put_time(std::gmtime(&scoreboard[i].first), "%M:%S") << std::endl;
   }

   return 0;
}
