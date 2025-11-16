#ifndef TUPLE_EXTENSION_H
#define TUPLE_EXTENSION_H

#include <ostream>
#include <type_traits>
#include <tuple>

template <typename T>
void print_elem(std::ostream &os, T elem) {
    // if constexpr(std::is_save<typename std::decay<T>::type, std::string>::value)
    if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
        os << "\"" << elem << "\"";
    } else {
        os << elem;
    }

}

template <std::size_t I, typename...Args>
struct tuple_printer {
    static void print(std::ostream &os, std::tuple<Args...> const &t) {
        tuple_printer<I-1, Args...>::print(os, t);  // firstly print previous argument
        os << ", ";
        print_elem(os, std::get<I>(t));
    }

};

template <typename ...Args> // base case. 1st argument
struct tuple_printer<0, Args...> {
    static void print(std::ostream &os, std::tuple<Args...> const &t) {
        os << std::get<0>(t);
    }
};


template <class CharT, class Traits, class...Args>
auto operator<<(std::basic_ostream<CharT, Traits> &os, std::tuple<Args...> const &t)
    -> std::basic_ostream<CharT, Traits> & {
    os << '(';
    if constexpr (sizeof...(Args)) {
        tuple_printer<sizeof...(Args) - 1, Args...>::print(os, t);
    }
    os << ')';
    return os;
}
#endif //TUPLE_EXTENSION_H
