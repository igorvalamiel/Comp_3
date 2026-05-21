#include <iostream>

using namespace std;

class Canvas;
class Papel;
class Tela;

class Figura {
    public:
        virtual void desenha( const Canvas* ) const = 0;
        virtual void desenha( const Papel* ) const = 0;
        virtual void desenha( const Tela* ) const = 0;
};

class Triangulo: public Figura {
    public:
    virtual void desenha( const Canvas* c) const { c-> desenha()}
};

class Canvas {
    
};

int main(){
    Figura *f = new Quadrado();
    Canvas *c = new Papel();

    f->desenha(c);
}