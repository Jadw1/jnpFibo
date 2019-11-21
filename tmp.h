#ifndef FIBO_H
#define FIBO_H

#include <string>
#include <boost/dynamic_bitset.hpp>
#include <functional>

class Fibo {
public:
    Fibo();
    explicit Fibo(const std::string& str);
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

    void normalize();
    Fibo& operator += (const Fibo& b);
    Fibo& operator &= (const Fibo& b);
    Fibo& operator |= (const Fibo& b);
    Fibo& operator ^= (const Fibo& b);
    template<typename T, typename = typename std::enable_if<
            std::is_integral<T>::value &&
            !std::is_same<T, bool>::value &&
            !std::is_same<T, char>::value>::type>
    Fibo& operator <<= (T n);

    // operatory + firend, += nie friend

    Fibo& operator=(const Fibo& that);
    Fibo& operator=(Fibo&& that) noexcept;

    size_t length();
    int bitAt(size_t i) const;
    void insertWindowIntoResult(unsigned long j, std::vector<short> window);

    friend std::ostream& operator<<(std::ostream& stream, const Fibo& fibo);

    boost::dynamic_bitset<> fibset; // TODO usunac po testach

private:
    // boost::dynamic_bitset<> fibset; TODO odkomentowac

    static unsigned long long getFibNumber(size_t i);
    static size_t findK(unsigned long long n);

    void cutZeros();

    void doBitwiseOperation(const Fibo &b, const std::function<bool(bool, bool)> &f);
};

const Fibo operator+(Fibo a, const Fibo& b); // TODO dodac reszte naglowkow

#endif //FIBO_H

//01001011
