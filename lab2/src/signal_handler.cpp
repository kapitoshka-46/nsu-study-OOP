#include "signal_handler.h"
#include <csignal>

namespace sig_handler {
    void SigHandler::Init() {
        is_stopping_ = false;
        //std::signal(SIGTERM, Handler_);
        std::signal(SIGINT, Handler_);
    }

    bool SigHandler::IsStopping() {
        return is_stopping_;
    }

    void SigHandler::Handler_(int signal) {
        is_stopping_ = true;
    }

}
