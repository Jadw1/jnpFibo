#include <iostream>
#include <cassert>

// tu wklej swoja klase
#include "fibo.h"

using namespace std;

const int n = 10;

int tab[n];
string str[n];

int main()
{

    // Poprawnościowe małe

    for (int i=0; i<n; i++) {
        cin >> tab[i];
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<=i; j++) {
            assert((Fibo(tab[i]) + Fibo(tab[j])) == Fibo(tab[i]+tab[j]));
            assert((Fibo(tab[i]) < Fibo(tab[j])) == (tab[i] < tab[j]));
        }
    }

    for (int i=0; i<n; i++) {
        cout << Fibo(tab[i]).length() << " " << Fibo(tab[i]) << "\n";
    }

    return 0;
    // Wydajnosciowe

    for (int i=0; i<n; i++) {
        cin >> str[i];
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<=i; j++) {
            cout << (Fibo(str[i]) + Fibo(str[j])) << "\n";
            cout << (Fibo(str[i]) | Fibo(str[j])) << "\n";
            cout << (Fibo(str[i]) ^ Fibo(str[j])) << "\n";
            cout << (Fibo(str[i]) & Fibo(str[j])) << "\n";
        }
    }
}
