#include "fibo.h"
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>

namespace {
    double phi() {
        static double phi = (1 + std::sqrt(5)) / 2;
        return phi;
    }

    size_t findK(unsigned long long n) {
        if(n == 0) {
            return 0;
        }

        double numerator = std::log(n * std::sqrt(5) + 0.5);
        static double dominator = std::log(phi());

        return std::floor(numerator/dominator) - 2;
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
}


Fibo::Fibo() : fibset(0) {

}

Fibo::Fibo(std::string str) : Fibo() {
    //TODO: remove leading 0
    for (int i = str.length() - 1; i >= 0; i--) {
        fibset.push_back(str[i] == '1');
    }

    normalize();
}

/*Fibo::Fibo(unsigned long long n) : fibset(findK(n) + 1) {
    while(n != 0) {
        size_t k = findK(n);
        auto fibNum = getFibNumber(k);
        fibset[k] = 1;
        n -= fibNum;
    }
}*/

/*template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
Fibo::Fibo(T n) : fibset(findK(n) + 1) {
    while(n != 0) {
        size_t k = findK(n);
        auto fibNum = getFibNumber(k);
        fibset[k] = 1;
        n -= fibNum;
    }
}*/

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

size_t Fibo::length() {
    return fibset.size();
}

std::ostream& operator<<(std::ostream& stream, const Fibo& fibo) {
    stream << fibo.fibset;
    return stream;
};

void Fibo::normalize() {

    size_t i = fibset.size()-1;
    while(fibset[i] == 0) --i;
    size_t safeSpot = i+1;

    for(i; i >= 1;){
        int j = i;
        while(j-1 >= 0 && fibset[j] == 1 && fibset[j-1] == 0){
            j-=2;
        }
        if(j >= 1 && fibset[j] == 1 && fibset[j-1] == 1) {
            fibset[j] = 0;
            fibset[j-1] = 0;

            for(size_t k = safeSpot-1; k > j+1;){
                fibset[k] = 0;
                k -= 2;

            }
            if(safeSpot == fibset.size()) fibset.push_back(0);
            fibset[safeSpot] = 1;
        }
        j >= 1 ? safeSpot = j-1 : safeSpot = 0;
        if(j >= 2) i = j-2;
        else i = 0;
    }
}

unsigned long long Fibo::getFibNumber(size_t i) {
    static std::vector<unsigned long long> fib;

    if (i >= fib.size()) {
        completeFibNumbers(fib, i);
    }

    return fib[i];
}


int bitAt(size_t i, boost::dynamic_bitset<> const &c){
    if(i < c.size()) return c[i];
    else return 0;
}
void changeWindow(size_t i, std::vector<short>& window){
    if(window[i] == 0) {
        if (window[i - 1] == 2) {
            if (window[i - 2] == 0) { //020x → 100(x++)
                window[i] = 1;
                window[i - 1] = 0;
                window[i - 2] = 0;
                window[i - 3]++;
            } else if (window[i - 2] == 1) { //021x → 110x
                window[i] = 1;
                window[i - 1] = 1;
                window[i - 2] = 0;
            }
        } else if (window[i - 1] == 3 && window[i - 2] == 0){ //030x → 110(x++)
            window[i] = 1;
            window[i - 1] = 1;
            window[i - 2] = 0;
            window[i - 3]++;
        } else if (window[i - 1] == 1 && window[i - 2] == 0){ //012x → 101x
            window[i] = 1;
            window[i - 1] = 0;
            window[i - 2] = 1;
        }
    }
}
void correctLastWindow(std::vector<short>& window){
    if(window.at(0) == 3){ //  03 can be changed to 11
        window[1] = 1;
        window[0] = 1;
    } else if(window.at(0) == 2){
        if(window.at(1) == 0) { //  02 can be changed to 10
            window[1] = 1;
            window[0] = 0;
        } else if(window.at(1) == 1){    // 012 can be changed to 101
            window[2] = 1;
            window[1] = 0;
            window[0] = 1;
        }
    } else if(window.at(0) == 0){
        if(window.at(1) == 3) { // 030 can be changed to 111
            window[2] = 1;
            window[1] = 1;
            window[0] = 1;
        } else if(window.at(1) == 2){  // 020 can be changed to 101
            if(window.at(2) == 0) {
                window[2] = 1;
                window[1] = 0;
                window[0] = 1;
            } else if(window.at(2) == 1){ //0120 can be changed to 1010
                window[3] = 1;
                window[2] = 0;
                window[1] = 1;
                window[0] = 0;
            }
        }
    }
}

Fibo operator + (Fibo a, Fibo b) {

    boost::dynamic_bitset<> const &c1 = a.getFibset();
    boost::dynamic_bitset<> const &c2 = b.getFibset();

    std::vector<short> vector;
    for (size_t i = 0; i < std::max(c1.size(), c2.size()); ++i) {
        vector.push_back(bitAt(i, c1) + bitAt(i, c2));
    }

    if (vector.at(vector.size() - 1) != 0) vector.push_back(0);
    for (size_t j = vector.size() - 1; j >= 3; --j) {
        changeWindow(j, vector);
    }
    correctLastWindow(vector);

    std::reverse(vector.begin(), vector.end()); // troche gówno
    std::ostringstream oss;
    if (!vector.empty()) {
        std::copy(vector.begin(), vector.end(),std::ostream_iterator<int>(oss, ""));
    }
    std::string s = oss.str(); // dotąd

    Fibo result(s);
    result.normalize();

    return result;
}

void Fibo::cutZeros() {
    size_t first1 = 0;
    for(size_t i = fibset.size() - 1; i >= 0; i--) {
        if(fibset[i]) {
            first1 = i;
            break;
        }
    }

    fibset.resize(first1 + 1, false);
}

int main(int, char* []) {
    Fibo a(4);
    Fibo b('r');
    Fibo c(false);
    Fibo d("10101");



    std::cout << findK(1) << std::endl;
    std::cout << findK(2) << std::endl;
    std::cout << findK(0) << std::endl;
    std::cout << findK(28) << std::endl << "koniec\n";

    std::string r = "12345";
    for (int i = 0; i < r.length(); i++) {
        std::cout << r[i];
    }

    boost::dynamic_bitset<> x(3); // all 0's by default
    x[0] = 1;
    x[1] = 1;
    std::cout << std::endl << x << std::endl;
    x[2] = 1;
    /*x[3] = 1;
    x[4] = 0;
    x[5] = 1;
    x[6] = 0;
    x[7] = 1;*/
    std::cout << x << "\n";
    //x = normalize(x);
    std::cout << x << "\n";

    std::cout << std::endl;
    std::cout << std::endl;

    boost::dynamic_bitset<> y(2); // all 0's by default
    y[0] = 0;
    y[1] = 1;
    std::cout << "toString " << y << std::endl;
    std::cout << "y[0] " << y[0] << " y[1] " << y[1] << std::endl;


    Fibo b("100100");
    Fibo c("1010101");

    Fibo a = b+c;
    std::cout << a.getFibset() << std::endl;
}
//Fibo f1      - tworzy liczbę 0
//
//Fibo f1(str) - tworzy liczbę na podstawie napisu str, który jest zapisem tej
//               liczby w systemie Fibonacciego (zapis niekoniecznie musi być
//               w postaci unormowanej)
//
//Fibo(n)      - tworzy liczbę równą liczbie całkowitej n
//
//Fibo f1(f2)  - konstruktor kopiujący
//
//f1 = f2 - przypisanie
//
//f1 + f2 - dodawanie
//
//f1 & f2 - and fibitowy (na postaci unormowanej)
//
//f1 | f2 - or fibitowy (na postaci unormowanej)
//
//f1 ^ f2 - xor fibitowy (na postaci unormowanej)
//
//f1 << n  - przesunięcie fibitowe w lewo o n pozycji
//
//f1 += f2
//
//f1 &= f2
//
//f1 |= f2
//
//f1 ^= f2
//
//f1 <<= n
//
//f1 op f2 - operatory porównujące wartości liczbowe reprezentowane przez f1 i f2,
//           gdzie op to jeden z operatorów: ==, <, <=, != , >, >=
//
//os << f1 - wypisuje f1 na strumień os bez żadnych białych znaków w postaci
//           unormowanej
//
//f1.length() - zwraca długość zapisu w postaci unormowanej liczby f1
//
//Ponadto należy zaimplementować globalne funkcje:
//
//Zero() - zwraca obiekt reprezentujący liczbę 0
//
//One()  - zwraca obiekt reprezentujący liczbę 1