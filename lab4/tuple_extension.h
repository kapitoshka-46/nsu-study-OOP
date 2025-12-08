#ifndef TUPLE_EXTENSION_H
#define TUPLE_EXTENSION_H

#include <iterator>
#include <ostream>
#include <type_traits>
#include <tuple>


// --------------------------- Tuple Printer ---------------------------//
template <typename T>
void print_elem(std::ostream &os, T elem) {
    os << elem;
}

template</*string*/>
inline void print_elem(std::ostream &os, std::string elem) {
    os << "\"" << elem << "\"";
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
    static void print(std::ostream &os, std::tuple<Args ...> const &t) {
        os << std::get<0>(t);
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
template <class CharT, class Traits, class...Args>
std::basic_istream<CharT, Traits> &
operator>>(std::basic_istream<CharT, Traits> &is, std::tuple<Args...> &t) {
    if constexpr (sizeof...(Args)) {
        auto reader = [&is](auto &... args) {
            (is >> ... >> args);    // == is >> arg1 >> arg2 >> arg3 >> ...
        };

        std::apply(reader, t);
    }
    return is;
}



// ---------------------- Tuple Reader ----------------------------- //
// template <typename T>
// void read_elem(std::istream &is, T &elem) {
//     is >> elem;
// }
//
// template <std::size_t I, typename...Args>
// struct tuple_reader {
//     static void read(std::istream &is, std::tuple<Args...> &t) {
//         tuple_reader<I-1, Args...>::read(is, t);  // firstly read previous argument
//         read_elem(is, std::get<I>(t));
//     }
// };
//
// template </*I=0,*/ typename ...Args> // base case. 1st argument
// struct tuple_reader<0, Args...> {
//     static void read(std::istream &is, std::tuple<Args ...> &t) {
//         read_elem(is, std::get<0>(t));
//     }
// };
//
//
// template <class CharT, class Traits, class...Args>
// std::basic_istream<CharT, Traits> &
// operator>>(std::basic_istream<CharT, Traits> &is, std::tuple<Args...> &t) {
//     if constexpr (sizeof...(Args)) {
//         tuple_reader<sizeof...(Args) - 1, Args...>::read(is, t);
//     }
//     return is;
// }

#endif //TUPLE_EXTENSION_H
