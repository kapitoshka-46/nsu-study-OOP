#include <iostream>
#include <thread>
#include <fstream>

#include "game-controller/GameController.h"
#include "signal-handler/signal_handler.h"

volatile bool is_stopping = false;

int main(int argc, char * argv[]) {



    game_controller::GameController game{argc, argv};
    game.Start();

    return 0;
}