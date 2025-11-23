#include "signal_handler.h"
#include <csignal>

namespace sig_handler {
    void SigHandler::Init() {
        is_stopping_ = false;
        std::signal(SIGTERM, Handler);
        std::signal(SIGINT, Handler);
    }

    bool SigHandler::IsStopping() {
        return is_stopping_;
    }

    void SigHandler::Handler(int /*signal*/) {
        is_stopping_ = true;
    }

}
