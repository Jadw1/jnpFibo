#include "fibo.h"
#include <boost/dynamic_bitset.hpp>
#include <utility>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include <functional>

namespace {
    double phi() {
        static double phi = (1 + std::sqrt(5)) / 2;
        return phi;
    }

    void completeFibNumbers(std::vector<unsigned long long>& fib, size_t to) {
        if (fib.empty()) {
            fib.push_back(1);
            fib.push_back(2);
        }

        for (size_t i = fib.size() - 1, j = fib.size() - 2; i <= to; i++, j++) {
            fib.push_back(fib[i] + fib[j]);
        }
    }

    void changeWindow(std::vector<short>& window) {
        if (window[0] == 0) {
            if (window[1] == 2) {
                if (window[2] == 0) { //020x → 100(x++)
                    window[0] = 1;
                    window[1] = 0;
                    window[2] = 0;
                    window[3]++;
                }
                else if (window[2] == 1) { //021x → 110x
                    window[0] = 1;
                    window[1] = 1;
                    window[2] = 0;
                }
            }
            else if (window[1] == 3 && window[2] == 0) { //030x → 110(x++)
                window[0] = 1;
                window[1] = 1;
                window[2] = 0;
                window[3]++;
            }
            else if (window[1] == 1 && window[2] == 2) { //012x → 101x
                window[0] = 1;
                window[1] = 0;
                window[2] = 1;
            }
        }
    }

    void correctLastWindow(std::vector<short>& window) {
        if (window[3] == 3) { //  xy03 → xy11
            window[2] = 1;
            window[3] = 1;
        }
        else if (window[3] == 2) {
            if (window[2] == 0) { //  xy02 → to xy10
                window[2] = 1;
                window[3] = 0;
            }
            else if (window[2] == 1) {    // x012 → x101
                window[1] = 1;
                window[2] = 0;
                window[3] = 1;
            }
        }
        else if (window[3] == 0) {
            if (window[2] == 3) { // x030 → x111
                window[1] = 1;
                window[2] = 1;
                window[3] = 1;
            }
            else if (window[2] == 2) {  // x020 → x101
                if (window[1] == 0) {
                    window[1] = 1;
                    window[2] = 0;
                    window[3] = 1;
                }
                else if (window[0] == 0 && window[1] == 1) { // 0120 → 1010
                    window[0] = 1;
                    window[1] = 0;
                    window[2] = 1;
                    window[3] = 0;
                }
            }
        }
        else if (window[1] == 0 && window[2] == 2 && window[3] == 1) {
            window[1] = 1;
            window[2] = 1;
            window[3] = 0;
        }
    }
}

Fibo::Fibo() : fibset(1) {}

Fibo::Fibo(const std::string& str) : fibset() {
    assert(str[0] != '0'); //checking if string has leading 0
    for (int i = str.length() - 1; i >= 0; i--) {
        assert(str[i] == '1' || str[i] == '0');
        fibset.push_back(str[i] == '1');
    }
    normalize();
}

Fibo::Fibo(const Fibo& that) : fibset(that.fibset) {

}

Fibo::Fibo(Fibo&& that) noexcept : fibset(std::move(that.fibset)) {

}

Fibo& Fibo::operator=(const Fibo& that) {
    fibset = that.fibset;
    return *this;
}

Fibo& Fibo::operator=(Fibo&& that) noexcept {
    fibset = std::move(that.fibset);
    return *this;
}

Fibo& Fibo::operator+=(const Fibo& b) {
    unsigned long maxLength = std::max(fibset.size(), b.fibset.size());
    Fibo result = Zero();
    result.fibset.resize(maxLength + 1, 0);
    if (maxLength + 1 < 4) {
        result.fibset.resize(4, 0);
        maxLength = 3;
    }
    std::vector<short> window(4);
    window[1] = this->bitAt(maxLength) + b.bitAt(maxLength);
    window[2] = this->bitAt(maxLength - 1) + b.bitAt(maxLength - 1);
    window[3] = this->bitAt(maxLength - 2) + b.bitAt(maxLength - 2);
    for (size_t j = maxLength; j >= 3; --j) {
        for (int i = 0; i <= 2; ++i) window[i] = window[i + 1];
        window[3] = this->bitAt(j - 3) + b.bitAt(j - 3);

        changeWindow(window);
        result.insertWindowIntoResult(j, window);
    }
    correctLastWindow(window);
    result.insertLastWindowIntoResult(window);

    result.normalize();
    result.trimLeadingZeros();

    this->fibset = result.fibset;
    return *this;

}

Fibo& Fibo::operator&=(const Fibo& b) {
    doBitwiseOperation(b, std::bit_and<>());
    return *this;
}

Fibo& Fibo::operator|=(const Fibo& b) {
    doBitwiseOperation(b, std::bit_or<>());
    return *this;
}

Fibo& Fibo::operator^=(const Fibo& b) {
    doBitwiseOperation(b, std::bit_xor<>());
    return *this;
}

Fibo& Fibo::operator<<=(size_t rhs) {
    fibset.resize(fibset.size() + rhs, false);
    fibset <<= rhs;
    return *this;
}

bool operator<(const Fibo& lhs, const Fibo& rhs) {
    if (lhs.length() == rhs.length()) {
        return lhs.fibset < rhs.fibset;
    }
    else {
        return lhs.length() < rhs.length();
    }
}

bool operator==(const Fibo& lhs, const Fibo& rhs) {
    return lhs.fibset == rhs.fibset;
}

size_t Fibo::length() const {
    return fibset.size();
}

std::ostream& operator<<(std::ostream& stream, const Fibo& fibo) {
    stream << fibo.fibset;
    return stream;
}

bool Fibo::bitAt(size_t i) const {
    if (i < fibset.size()) return fibset[i];
    else return false;
}

void Fibo::normalize() {
    long i = fibset.size() - 1;
    while (fibset[i] == 0 && i > 0) { // iterate through leading zeros
        --i;
    }
    size_t safeSpot = i + 1; // last place seen where 1 can be put without breaching normality condition

    while (i >= 1) {
        long j = i;
        while (j - 1 >= 0 && fibset[j] == 1 &&
               fibset[j - 1] == 0) { // iterate through "10" where there are no safe places
            j -= 2;
        }
        if (j >= 1 && fibset[j] == 0 && fibset[j - 1] == 1) {
            safeSpot = j;
            i = j - 1;
        }
        else if (j >= 1 && fibset[j] == 1 && fibset[j - 1] == 1) {
            fibset[j] = 0;
            fibset[j - 1] = 0;
            for (long k = safeSpot - 1; k > j + 1;) { // zero all ones from passed "10" series
                fibset[k] = 0;
                k -= 2;
            }

            if (safeSpot == fibset.size()) fibset.push_back(0);
            fibset[safeSpot] = 1;
            if (j >= 1) safeSpot = j - 1;
            if (j >= 2) i = j - 2;
            else i = 0;
        }
        else if (j >= 1 && fibset[j] == 0 && fibset[j - 1] == 0) {
            if (j >= 1) safeSpot = j - 1;
            if (j >= 2) i = j - 2;
            else i = 0;
        }
        else {
            i = 0;
        }
    }
}

void Fibo::trimLeadingZeros() {
    size_t first1 = 0;
    for (long long i = fibset.size() - 1; i >= 0; i--) {
        if (fibset[i]) {
            first1 = i;
            break;
        }
    }
    fibset.resize(first1 + 1, false);
}

void Fibo::doBitwiseOperation(const Fibo& b, const std::function<bool(bool, bool)>& f) {
    unsigned long maxLength = std::max(this->length(), b.length());
    fibset.resize(maxLength, false);
    for (unsigned long i = 0; i < maxLength; ++i) {
        fibset[i] = f(this->bitAt(i), b.bitAt(i));
    }
    trimLeadingZeros();
    normalize();
}

void Fibo::insertWindowIntoResult(unsigned long j, std::vector<short> window) {
    fibset[j] = window[0];
}

void Fibo::insertLastWindowIntoResult(std::vector<short> window) {
    for (int i = 0; i <= 3; ++i) fibset[3 - i] = window[i];
}

unsigned long long Fibo::getFibNumber(size_t i) {
    static std::vector<unsigned long long> fib;

    if (i >= fib.size()) {
        completeFibNumbers(fib, i);
    }

    return fib[i];
}

size_t Fibo::findK(unsigned long long n) {
    if (n == 0) {
        return 0;
    }

    double numerator = std::log(n * std::sqrt(5) + 0.5);
    static double denominator = std::log(phi());

    return std::floor(numerator / denominator) - 2;
}

const Fibo& Zero() {
    static const Fibo zero;
    return zero;
}

const Fibo& One() {
    static const Fibo one(1);
    return one;
}