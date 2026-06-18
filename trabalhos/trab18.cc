#include <iostream>
#include <vector>
#include "trab18_filter.cc"

using namespace std;

int main( int argc, char* argv[] ) {
    Intervalo( 9, 12 )  | []( auto x ) { cout << x << " "; };
    return 0;
}
