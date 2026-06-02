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

void printVar (Var v) {cout << '\n' << v.type() << '\n'; }
 
int main( int argc, char* argv[] ) try {
 
    /* TESTCASE-PLACE-HOLDER */
    Var a, b;
    a = newObject();
    printVar(a);
    b = "José Maria";
    printVar(b);
    a["nome"] = b;
    a["nascimento"] = 1998;
    b = "Maria José";
    try {
    print( a );
    } catch( Var::Erro e ) {
    cout << "Erro fatal: " << e() << endl;
    }
    cout << a << " " << a["nome"] << " " << a["nascimento"] << endl;
    
    return 0;
} catch( Var::Erro e ) {
    cout << "Erro fatal: " << e() << endl;
}

// { nome: José Maria, idade: Erro fatal: Essa variável não pode ser usada como função
// object José Maria 1998