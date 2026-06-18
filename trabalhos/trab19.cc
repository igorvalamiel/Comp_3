#include <iostream>
#include <vector>
#include "trab19_var.cc"

using namespace std;

int main() {

    Var a, b;
    a = newArray();
    
    // array também pode ter propriedades string
    a["sqr"] = [](int n){ return n*n; };
    
    for( b = 0; (b < 10).asBool(); b = b + 1 )
    a[b] = a["sqr"]( b ); // a[0]=0, a[1]=1, a[2]=4, ..., a[9]=81

    // predicado: retorna true para números pares
    Var pares = [](int n){ return n%2==0; };
    
    // filter + forEach
    a.filter( pares ).forEach( [](Var n){ cout << n << " "; } );
    
    // filter + map + forEach
    a.filter( pares )
    .map( [](auto x){ return x / 2 + 0.1; } )
    .forEach( [](Var n){ cout << n << " "; } );
    
    // indexOf manual vs. nativo
    auto indexOf = [](const Var& array, Var valor) {
    int n = 0, pos = -1;
    array.forEach( [&n,&pos,valor](auto x) {
    if( pos == -1 ) {
    if( (x == valor).asBool() ) pos = n;
    n++;
    }
    } );
    return pos;
    };
    
    // ambos devem dar o mesmo resultado
    cout << (indexOf( a, 36 ) == a.indexOf( 36 ) ) << endl;
    
    // índice além do limite, depois indexOf
    a[11] = 'A';
    cout << a.indexOf( "A" ) << endl;

    return 0;
}