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
    Var a, b = 10;
    cout << a << " " << b << endl;
    a = 3.14;
    b = "uma string";
    cout << a << " " << b << endl;
    
    return 0;
} catch( Var::Erro e ) {
    cout << "Erro fatal: " << e() << endl;
}