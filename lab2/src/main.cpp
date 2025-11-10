#include <iostream>
#include <thread>
#include <fstream>

#include "file_handler.h"
#include "Core.h"

using namespace std;
using namespace game_core;
using enum CellType;
using namespace std::chrono_literals;
int main() {


    Universe u {20, 20};
    ifstream in;
    in.open("pulsar.lif");

    file_handler::LoadFromFile(in, u);
    std::cout << u.ToString();

    for (int i = 0; i < 10000; i++) {
        std::this_thread::sleep_for(400ms);
        u.Step();
        system("clear");
        std::cout << "\n\n";
        std::cout << u.ToString();
    }
    return 0;
}