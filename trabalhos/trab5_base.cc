#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <map>

#include "trab5_apply.cc"

using namespace std;

double seno( double n ) { return sin(n); }
int id( int n ) { return n; }
string roman( int n ) {
    map<int,string> rom = { { 1, "I" }, { 2, "II" }, { 3, "III" }, { 4, "IV" }, { 5, "V" }, { 6, "VI" }, { 7, "VII" }, { 8, "VIII" } } ; 

    return rom[n]; 
}

struct FunctorSimples { 
    string operator()( int n ) { return roman( n ) + "!"; }
};

struct FunctorTemplate { 
    template <typename T>
    T operator()( T n ) { return n+n; }
};

template <typename T>
ostream& operator << ( ostream& o, const vector<T>& v ) {
    o << "[ ";
    for( auto x : v )
        o << x << " ";
    
    return o << "]";
}

int main( int argc, char* argv[]) {     

    auto v = { 1, 2, 3, 4, 5 };
    cout << ::apply( v, []( auto n ) -> double { return n*n + 0.1; } );

    return 0;
}