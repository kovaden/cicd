Print IP representation for an integral type.

This is a header-only library which prints IP-like things represented in various format.
No particular usage except for learning purposes. Also it has a sample test program which demonstrates all that downloads.

It has 4 overloads for a function print_ip.

For printing IP represented as a value of integral type:
```
template<typename  T,
        typename = std::enable_if_t<std::is_integral_v<T>, T>>
void print_ip(T inp);
```

For printing IP represented as a string value:
```
template<typename  T,
        typename std::enable_if<std::is_same_v<T, std::string>, T>::type * = nullptr >
void print_ip(const T &inp);
```

For printing IP represented as a container, vector or string:
```
template<typename  T,
        typename std::enable_if<std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::list<short>>, T>::type * = nullptr >
inline void print_ip(const T &inp);
```

And, finally, to print IP represented as a std::tuple
```
template<std::size_t I = 0, typename ...Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
print_ip(const std::tuple<Tp...> &t);
```

Please consult print_ip.cpp for usage examples.

To try it:

1. Make a build directory and cd to it:
```
mkdir build && cd build
```

2. Run cmake
```
cmake
```

3. Run make
```
make 
```

4. And finally run the program:
```
./print_ip
```
