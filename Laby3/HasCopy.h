#include <iostream>
#include <vector>
#include <typeinfo>

template <typename T>
class HasCopy {
private:
    typedef char YesType[1];
    typedef char NoType[2];

    template <typename U>
    static YesType& check(decltype(&U::copy));

    template <typename U>
    static NoType& check(...);

public:
    static const bool value = sizeof(check<T>(&T::copy)) == sizeof(YesType);
};
