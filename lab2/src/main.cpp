#include <iostream>
#include <thread>
#include <fstream>

#include "GameController.h"


int main(int argc, char * argv[]) {

    game_controller::GameController game{argc, argv};
    game.Start();

    return 0;
}