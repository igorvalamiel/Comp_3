#include <iostream>

using namespace std;

class Figura {
    public: 
        Figura (double x=0, double y=0): x(x), y(y) {

        }

        virtual void desenha() {
            cout << "Nao implementado - Abstrato" << endl;
        }

        virtual ~Figura() {
            
        }
    private:
        double x, y;
};

class Quadrado: public Figura {
    public:
        Quadrado(double x, double y, double lado): Figura(x,y), lado(lado){

        }

        void desenha() {
            cout << "Desenhou um quadrado de lado " << lado << endl;
        }

    private:
        double lado;
};

void foo( Figura& f) {
    cout << "Tamanho de f: " << sizeof(f) << endl;
    f.desenha();
}

int main() {
    Quadrado a(0, 1, 10);
    Figura b(0, 1);

    foo(a);
    foo(b);

    cout << sizeof(a) << endl;
    cout << sizeof(b) << endl;
    //cout << &a << endl;

    return 0;
}
