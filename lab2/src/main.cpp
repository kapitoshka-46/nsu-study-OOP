#include "game-controller/GameController.h"


int main(int argc, char * argv[]) {

    game_controller::GameController game{argc, argv};
    game.Start();

    return 0;
}