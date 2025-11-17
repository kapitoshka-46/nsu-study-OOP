#include "GameController.h"

#include <fstream>
#include <iostream>

#include "../file-handler/file_handler.h"
#include "../../libs/cxxopts/cxxopts.hpp"

namespace game_controller {
    void GameController::RunInteractive() {
        term = new terminal::Terminal(std::cin, std::cout);
        if (input_file_.empty()) {
            term->ExecuteCommand("random");
        }
        else {
            term->ExecuteCommand("load " + input_file_);
        }
        term->RunLoop();

    }

    void GameController::Log(const std::string &msg) const {
        if (is_verbose_) {
            std::cout << msg << '\n';
        }
    }

    GameController::GameController(int argc, char *argv[]) {
        cxxopts::Options options("game_of_life", "helps_string");

        // what a nice parser :-)
        options.add_options()
            ("o,output",  "Output file", cxxopts::value<std::string>(output_file_))
            ("i,iterations", "Num of iterations", cxxopts::value<int>(iterations_))
            ("h,help",  "Print help")
            ("file", "Input file", cxxopts::value<std::string>(input_file_))
            ("v, verbose", "Verbose", cxxopts::value<bool>(is_verbose_));

        options.parse_positional("file");

        auto result = options.parse(argc, argv);

        if (result.contains("help")) {
            std::cout << options.help();
            mode_ = Mode::kExit;
            return;
        }
        if (result.contains("iterations")) {
            if (not result.contains("file") or not result.contains("output")) {
                std::cout << "You should specify input and output files\n";
                mode_ = Mode::kExit;
                return;
            }
            mode_ = Mode::kOfflineIterations;
            return;
        }

        /* "game --output=out" is invalid case because
         * it runs interactive mode which will ignore output file */
        if (not result.contains("iterations") and result.contains("output")) {
            std::cout << "If you specify the output file, you should provide the num of iterations with option -i <num> or --iterations=<num>\n";
            mode_ = Mode::kExit;
            return;
        }
        mode_ = Mode::kInteractive;
    }

    void GameController::RunOffline() const {
        if (input_file_.empty()) {
            throw std::logic_error("No input file");
        }
        if (output_file_.empty()) {
            throw std::logic_error("No output file");
        }
        core::universe universe;
        std::ifstream in {input_file_};
        std::ofstream out {output_file_};

        Log("Loading Universe.");
        file_handler::LoadFromFile(in, universe);
        Log("Start iterations.");
        for (int i = 0; i < iterations_; i++) {
            universe.Step();
        }
        Log("Done all iterations.");
        Log("Saving Universe.");
        file_handler::SaveToFile(out, universe);
        Log("Universe saved");
    }

    void GameController::Start() {
        switch (mode_) {
            case Mode::kInteractive: {
                RunInteractive();
                break;
            }
            case Mode::kExit: {
                return;
            }
            case Mode::kOfflineIterations: {
                RunOffline();
                break;
            }

            default: {
                throw std::invalid_argument("Unknown game mode");
            };
        }
    }

    GameController::~GameController() {
        delete term;
    }
} // game_controller