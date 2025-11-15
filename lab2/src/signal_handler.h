#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H



namespace sig_handler {
    class SigHandler {
    public:
        static volatile inline bool is_stopping_ = false;

        static void Init(); // inits sig handler
        static bool IsStopping();

    private:
        static void Handler_(int signal);
    };
}

#endif //SIGNAL_HANDLER_H
