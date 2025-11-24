#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include <string>

#include "../Terminal/Terminal.h"

namespace game_controller {

    enum class Mode {
        kOfflineIterations,
        kInteractive,
        kExit,
    };

class GameController {
    Mode mode_ = Mode::kInteractive;
    std::string input_file_;
    std::string output_file_;
    int iterations_ = 0;
    terminal::Terminal* term = nullptr; // for interactive mode
    bool is_verbose_ = false;

    void RunInteractive();

    void RunOffline() const;

    void Log(const std::string &msg) const;

public:
    GameController(int argc, char* argv[]);     // parses input args

    void Start();   // starts the game (in any mode)

    ~GameController();

};

} // game_controller

#endif //GAMECONTROLLER_H
