#ifndef TUPLE_EXTENSION_H
#define TUPLE_EXTENSION_H

#include <iterator>
#include <ostream>
#include <tuple>
#include <spdlog/spdlog.h>

// --------------------------- Tuple Printer ---------------------------//
template <class CharT, class Traits, class T>
concept Streamable = requires(std::basic_ostream<CharT, Traits> os, T t)
{
    os << t;
};



template <class CharT, class Traits, class T>
void print_elem(std::basic_ostream<CharT, Traits> &os, T elem) {
    os << elem;
}

template <class CharT, class Traits>
void print_elem(std::basic_ostream<CharT, Traits> &os, const std::basic_string<CharT, Traits> &elem) {
    os << "\"" << elem << "\"";
}

template <std::size_t I, class CharT, class Traits, class...Args>
struct tuple_printer {
    static void print(std::basic_ostream<CharT, Traits> &os, std::tuple<Args...> const &t) {
        using T = std::tuple_element_t<I, std::tuple<Args...>>;
        static_assert(
            Streamable<CharT, Traits, T>,
            "Tuple element type for param is not supports shift to basic_ostream. "
            "Overload operator<<(basic_ostream<CharT, Traits> &, const YourType &) for all types that you use in tuple."
            );

        tuple_printer<I-1, CharT, Traits, Args...>::print(os, t);  // firstly print previous argument
        os << " | ";

        print_elem(os, std::get<I>(t));
    }
};

template <class CharT, class Traits, class ...Args>     // base case: I == 0
struct tuple_printer<0, CharT, Traits, Args...> {
    static void print(std::basic_ostream<CharT, Traits> &os, std::tuple<Args ...> const &t) {
        using T = std::tuple_element_t<0, std::tuple<Args...>>;
        static_assert(
                    Streamable<CharT, Traits, T>,
                    "Tuple element type for param is not supports shift to basic_ostream. "
                    "Overload operator<<(basic_ostream<CharT, Traits> &, const YourType &) for all types that you use in tuple."
                    );
        print_elem(os, std::get<0>(t));
    }
};



template <class CharT, class Traits, class...Args>
std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &os, std::tuple<Args...> const &t) {
    os << '(';
    if constexpr (sizeof...(Args)) {
        tuple_printer<sizeof...(Args) - 1, CharT, Traits, Args...>::print(os, t);
    }
    os << ')';
    return os;
}


// ---------------------- Tuple Reader ----------------------------- //
template <class CharT, class Traits, class T>
concept ShiftedFromStream = requires(std::basic_istream<CharT, Traits> is, T t)
{
    is >> t;
};


template <class CharT, class Traits, typename T>
T from_string(const std::basic_string<CharT, Traits> &str) {
    if constexpr (std::is_same_v<T, std::basic_string<CharT, Traits>>) {
        return str;
    }
    else {
        std::basic_istringstream<CharT, Traits> iss(str);
        T res {};
        iss >> res;
        return res;
    }

}


template <class CharT, class Traits, class...Args>
std::tuple<Args...>
make_tuple_from_strings(const std::vector<std::basic_string<CharT, Traits>> &fields)
requires (ShiftedFromStream<CharT, Traits, Args> && ...)
{
    if (fields.size() < sizeof...(Args)) {
        throw std::invalid_argument("Not enough params for making a tuple");
    }

    if (fields.size() < sizeof...(Args)) {
        throw std::invalid_argument("Too many params for making a tuple");
    }
    std::tuple<Args...> t;


    if constexpr (sizeof...(Args)) {
        size_t i = 0;
        auto maker = [&i, fields] (Args&... args) {
            ((args = from_string<CharT, Traits, std::decay_t<Args>>(fields.at(i++))),...);
        };

        std::apply(maker, t);
    }
    return t;
}
#endif //TUPLE_EXTENSION_H
