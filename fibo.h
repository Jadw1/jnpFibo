#ifndef FIBO_H
#define FIBO_H

#include <string>
#include <boost/dynamic_bitset.hpp>
#include <functional>
#include <boost/operators.hpp>

class Fibo : boost::addable<Fibo>, boost::totally_ordered<Fibo>,
    boost::bitwise<Fibo>, boost::left_shiftable<Fibo, unsigned long long> {
public:
    Fibo();
    explicit Fibo(const std::string& str);
    template<typename T, typename = typename std::enable_if<
        std::is_integral<T>::value &&
        !std::is_same<T, bool>::value &&
        !std::is_same<T, char>::value>::type>
    Fibo(T n) : fibset(findK(n) + 1) {
        assert(n >= 0);

        while(n != 0) {
            size_t k = findK(n);
            auto fibNum = getFibNumber(k);
            fibset[k] = 1;
            n -= fibNum;
        }
    }
    Fibo(const Fibo& that);
    Fibo(Fibo&& that) noexcept;

    Fibo& operator=(const Fibo& that);
    Fibo& operator=(Fibo&& that) noexcept;

    Fibo& operator += (const Fibo& b);
    Fibo& operator &= (const Fibo& b);
    Fibo& operator |= (const Fibo& b);
    Fibo& operator ^= (const Fibo& b);
    Fibo& operator <<= (size_t rhs);
    friend bool operator<(const Fibo& lhs, const Fibo& rhs);
    friend bool operator==(const Fibo& lhs, const Fibo& rhs);

    size_t length() const;

    friend std::ostream& operator<<(std::ostream& stream, const Fibo& fibo);

private:
    boost::dynamic_bitset<> fibset;
    bool bitAt(size_t i) const;
    void normalize();
    void trimLeadingZeros();
    void doBitwiseOperation(const Fibo &b, const std::function<bool(bool, bool)> &f);
    void insertWindowIntoResult(unsigned long j, std::vector<short> window);
    void insertLastWindowIntoResult(std::vector<short> window);

    static unsigned long long getFibNumber(size_t i);
    static size_t findK(unsigned long long n);
};

const Fibo& Zero();
const Fibo& One();

#endif //FIBO_H
