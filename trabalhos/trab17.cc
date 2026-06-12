#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
 
using namespace std;
 
#include "trab17_bind.cc"
 
int main() {
    
    /* TESTCASE-PLACE-HOLDER */
    auto f2 = ::bind( mdc, __ );
    auto f1 = ::bind( f2, __, 12 );
    for( int i = 2; i <= 12; i++ )
    cout << f1( i ) <<  " ";
    
    return 0;
}