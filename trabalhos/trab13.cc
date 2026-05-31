#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include <type_traits>
#include <functional>
#include "trab13_var.cc"
 
using namespace std;
 
Var print( const Var& o ) {
    cout << "{ nome: " << o["nome"] << ", idade: " << o["idade"]( o ) << ", nascimento: " << o["nascimento"] << ", print: " << o["print"] << ", atr: " << o["atr"] << " }" << endl;
    return Var();   
}
 
void imprime( Var v ) { v["print"]( v ); }
 
int main( int argc, char* argv[] ) try {
 
    /* TESTCASE-PLACE-HOLDER */
    Var a[5] = { true, 'X', 2, 2.1, "abracadabra" };
    Var b = 200, c = 300.1, d = "palavra ";
    for( auto& x: a ) {
        cout << x+b << "," << x+c << "," << x+d << "," << 3 / x << "," << 1.1 * x << "," 
            << Var(x && true) << "," << Var(x && false) <<  "," << Var(b >= x) << "," << Var(x < d) << endl;
    }
    
    return 0;
} catch( Var::Erro e ) {
    cout << "Erro fatal: " << e() << endl;
}