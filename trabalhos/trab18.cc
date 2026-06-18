#include <iostream>
#include <vector>
#include "trab18_filter.cc"

using namespace std;

 


int main( int argc, char* argv[] ) {
    int tab[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<int> v;

    tab | [ &v ]( int x ) { v.push_back( x ); };

    v | []( int    x ) { return x % 2 == 0;   }   // filter: pares
        | []( int    x ) { return x*x / 10.0; }   // map: x² / 10
        | []( double x ) { cout << x << endl; };  // sink

    cout << endl;
    return 0;
}