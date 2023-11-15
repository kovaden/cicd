//
// Created by denis on 13.10.23.
//

#ifndef PRINT_IP_PRINT_IP_H
#define PRINT_IP_PRINT_IP_H

#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <array>
#include <iostream>
#include <type_traits>
#include <cstdint>

/// \brief Print IP representation for an integral type.
///
///   This function prints IP address represented as C++ integral type
///
/// <typeparam name="T"> T The Type of the parameter to be printed.
/// \param inp Input integer to be printed</param>
/// \return Nothing.
template<typename  T,
        typename = std::enable_if_t<std::is_integral_v<T>, T>>
void print_ip(T inp) {
    union _u {
        T value;
        std::array<uint8_t, sizeof(T)> bytes;
    } u = {inp};

    for (size_t i = 0; i < u.bytes.size(); i++){
        if (i != 0) {
            std::cout << ".";
        }
        std::cout << +u.bytes[u.bytes.size() - i - 1];
    }
    std::cout << std::endl;
}

/// \brief Print IP representation for std::string.
///
///   This function prints IP address represented as a string
///
/// <typeparam name="T"> T is the Type of the parameter to be printed.
/// \param inp Input string to be printed</param>
/// \return Nothing.
template<typename  T,
        typename std::enable_if<std::is_same_v<T, std::string>, T>::type * = nullptr >
void print_ip(const T &inp) {
    std::cout << inp << std::endl;
}



/// \brief Print IP representation for a container.
///
///   This function prints IP address represented as a string
///
/// <typeparam name="T"> T is the Type of the parameter to be printed.
/// \param inp Input string to be printed</param>
/// \return Nothing.

template<typename  T,
        typename std::enable_if<std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::list<short>>, T>::type * = nullptr >
inline void print_ip(const T &inp) {
    for (auto b = inp.begin(); b != inp.end(); b++) {
        if (b != inp.begin()) {
            std::cout << ".";
        }
        std::cout << *b;
    }
    std::cout << std::endl;
}

// Recursively print a tuple
template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
print_ip(const std::tuple<Tp...> &t)
{
    (void)t;
    std::cout << std::endl;
}

/// \brief Print IP representation for std::tuple.
///
///   This function prints IP address represented as std::tuple
///
/// <typeparam name="T"> T is the Type of the parameter to be printed.
/// \param inp Input string to be printed</param>
/// \return Nothing.

template<std::size_t I = 0, typename ...Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
print_ip(const std::tuple<Tp...> &t)
{
    static_assert(std::is_same_v<
                  std::tuple_element_t<0, std::remove_reference_t<decltype(t)>>,
                  std::tuple_element_t<I, std::remove_reference_t<decltype(t)>>
    >);

    std::cout << std::get<I>(t);
    if (I < sizeof...(Tp)-1) {
        std::cout << ".";
    }
    print_ip<I+1, Tp...>(t);
}

#endif //PRINT_IP_PRINT_IP_H
