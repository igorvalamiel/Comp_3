#include <iostream>

using namespace std;

class Base {
    public:
        Base() : campo_int(0) {
            cout << "Chamou construtor de base" << endl;}

        ~Base() {
            cout << "Chamou construtor de base" << endl;}

    private:
        int campo_int;
};

class Derivada : public Base {
    public:
        Derivada(): Base(), x(0.8) {
            cout << "Chamou construtor de Derivada" << endl;}
        
        ~Derivada() {
            cout << "Chamou destrutor de Derivada" << endl;}

    private:
        int x;
};

int main() {

    Base a, b;
    cout << sizeof(a) << endl;

    return 0;
}