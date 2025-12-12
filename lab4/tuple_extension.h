#ifndef TUPLE_EXTENSION_H
#define TUPLE_EXTENSION_H

#include <iterator>
#include <ostream>
#include <tuple>
#include <spdlog/spdlog.h>

// --------------------------- Tuple Printer ---------------------------//
template <typename T>
void print_elem(std::ostream &os, T elem) {
    os << elem;
}

inline void print_elem(std::ostream &os, const std::string &elem) {
    os << "\"" << elem << "\"";
}

template <std::size_t I, typename...Args>
struct tuple_printer {
    static void print(std::ostream &os, std::tuple<Args...> const &t) {
        tuple_printer<I-1, Args...>::print(os, t);  // firstly print previous argument
        os << " | ";
        print_elem(os, std::get<I>(t));
    }
};

template <typename ...Args>     // base case: I == 0
struct tuple_printer<0, Args...> {
    static void print(std::ostream &os, std::tuple<Args ...> const &t) {
        print_elem(os, std::get<0>(t));
    }
};


template <class CharT, class Traits, class...Args>
std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &os, std::tuple<Args...> const &t) {
    os << '(';
    if constexpr (sizeof...(Args)) {
        tuple_printer<sizeof...(Args) - 1, Args...>::print(os, t);
    }
    os << ')';
    return os;
}


// ---------------------- Tuple Reader ----------------------------- //
template <typename T>
T from_string(const std::string &str) {
    std::istringstream iss(str);
    T res {0};
    iss >> res;
    return res;
}

template <>
inline std::string from_string(const std::string &str) {
    return str;
}

template <class CharT, class Traits, class...Args>
std::tuple<Args...>
make_tuple_from_strings(const std::vector<std::basic_string<CharT, Traits>> &fields) {
    std::tuple<Args...> t;
    // if fields.size != sizeof... Args

    if constexpr (sizeof...(Args)) {
        size_t i = 0;
        auto maker = [&i, fields] (Args&... args) {
            ((args = from_string<std::decay_t<Args>>(fields.at(i++))),...);
        };

        std::apply(maker, t);
    }
    return t;
}
#endif //TUPLE_EXTENSION_H
