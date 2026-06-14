#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
 
using namespace std;
 
#include "trab17_bind.cc"

long mdc( long a, long b ) { return b == 0 ? a : mdc( b, a%b ); }
int foo (int x, int y, int z) { return x+y+z;}
 
int main() {
    PlaceHolder __;
    
    /* TESTCASE-PLACE-HOLDER */
    auto f = ::bind( foo, __, __, 34 ); // terceiro parâmetro fixado em 34
    auto g = f( __, 56 ); // segundo fixado em 56 → equivale a bind( foo, __, 56, 34 )
    cout << g( 9 ) << endl; // chama foo( 9, 56, 34 )
    
    return 0;
}