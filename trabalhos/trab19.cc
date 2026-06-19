#include <iostream>
#include <vector>
#include "trab19_var.cc"

using namespace std;

int main() {

    Var tab[] = { Var(), true, 10, '2', 0, '0', 'X', newObject(), "", "palavra", " 0 " };

    //for( auto& a: tab ) cout << "Valor=" << a << ":" << a.asString() << "," << a.asBool() << "," << !!a << "," << Var( a.isNumber() ) << endl;

    int n = 0;
    cout << "Valor=" << tab[n] << ":" << tab[n].asString() << "," << tab[n].asBool() << "," << !!tab[n] << "," << Var( tab[n].isNumber() ) << endl;

    return 0;
}

/*
    Valor=undefined:undefined,0,false,false
    Valor=true:true,1,true,true
    Valor=10:10,1,true,true
    Valor=2:2,1,true,true
    Valor=0:0,0,false,true
    Valor=0:0,1,true,true
    Valor=X:X,1,true,false
    Valor=object:object,1,true,false
    Valor=:,0,false,true
    Valor=palavra:palavra,1,true,false
    Valor= 0 : 0 ,1,true,true
*/