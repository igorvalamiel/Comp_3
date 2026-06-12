#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <string.h>
 
using namespace std;
 
#include "trab16_lambda.cc"
 
vector<int> operator + ( const vector<int>& v, int n ) {
    vector<int> result{ v };
    result.push_back( n );
    return result;
}
 
int main() {
    decltype(::x) x(::x);
    
    /* TESTCASE-PLACE-HOLDER */
    vector< vector<int> > m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
    m | []( auto v ) { return v[0] != 0; } | ( x | cout << x << ' ' );
    
    return 0;
}
