#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H



namespace sig_handler {
    // handle signals to process like SIGINT
    class SigHandler {
    public:
        static volatile inline bool is_stopping_ = false;

        static void Init(); // inits sig handler
        static bool IsStopping();

    private:
        static void Handler(int signal);
    };
}

#endif //SIGNAL_HANDLER_H
