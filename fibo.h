#ifndef FIBO_H
#define FIBO_H

#include <string>
#include <boost/dynamic_bitset.hpp>

class Fibo {
public:
    Fibo();
    explicit Fibo(std::string str);
    template<typename T, typename = typename std::enable_if<
        std::is_integral<T>::value &&
        !std::is_same<T, bool>::value &&
        !std::is_same<T, char>::value>::type>
    explicit Fibo(T n) : fibset(findK(n) + 1) {
        while(n != 0) {
            size_t k = findK(n);
            auto fibNum = getFibNumber(k);
            fibset[k] = 1;
            n -= fibNum;
        }
    }
    Fibo(const Fibo& that);
    Fibo(Fibo&& that) noexcept;

    void normalize(); // TODO może nie potrzebne (dodane dla potrzeb operator+)
    boost::dynamic_bitset<> getFibset() {
        return fibset;
    }

    Fibo& operator=(const Fibo& that);
    Fibo& operator=(Fibo&& that) noexcept;

private:
    boost::dynamic_bitset<> fibset;

    static unsigned long long getFibNumber(size_t i);

    //void normalize(); // TODO może nie potrzebne (dodane dla potrzeb operator+)
    void cutZeros();
};

#endif //FIBO_H
