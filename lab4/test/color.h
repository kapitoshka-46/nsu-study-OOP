#ifndef COLOR_H
#define COLOR_H
namespace color {
    constexpr std::string reset   = "\033[0m";
    constexpr std::string bold    = "\033[1m";

    constexpr std::string red     = "\033[31m";
    constexpr std::string green   = "\033[32m";
    constexpr std::string yellow  = "\033[33m";
    constexpr std::string blue    = "\033[34m";
    constexpr std::string magenta = "\033[35m";
    constexpr std::string cyan    = "\033[36m";
    constexpr std::string white   = "\033[37m";

    constexpr std::string gray           = "\033[90m";
    constexpr std::string bright_red     = "\033[91m";
    constexpr std::string bright_green   = "\033[92m";
    constexpr std::string bright_yellow  = "\033[93m";
    constexpr std::string bright_blue    = "\033[94m";
    constexpr std::string bright_magenta = "\033[95m";
    constexpr std::string bright_cyan    = "\033[96m";
    constexpr std::string bright_white   = "\033[97m";
}
#endif //COLOR_H