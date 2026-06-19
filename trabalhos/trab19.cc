#include <iostream>
#include <vector>
#include "trab19_var.cc"

using namespace std;

int main() {

    Var a, sq = []( int n ){ return n*n; };

    a = newArray();

    for( Var b = 0; (b < 10).asBool(); b = b + 1 )
        a[b] = sq( b );

    for( int i = 0; i < 10; i++ )
        cout << a[i] << ";";

    cout << endl;
    return 0;
}
