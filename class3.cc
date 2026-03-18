#include <iostream>

using namespace std;

const int MAX_PILHA = 10;

class PilhaInt { 
  public:
    PilhaInt();
    PilhaInt( const PilhaInt& p );
    virtual void empilha( int valor );
    int desempilha();

    const PilhaInt& operator= ( const PilhaInt& p ){
        if( this != &p ) {
            atual = p.atual;
            for( int i = 0; i < p.atual; i++ ) {
                tab[i] = p.tab[i];
            }
        }
        return p;
    }

    PilhaInt& operator << (int valor) {
      empilha( valor );
      return *this;
    }

  private:
    int tab[MAX_PILHA];
    int atual;
};

PilhaInt::PilhaInt() {
    cout << "Chamou o construtor" << endl;
    for (int i=0; i<MAX_PILHA; i++){
            tab[i] = 0;
        }
        atual = 0;
};

void PilhaInt::empilha(int valor) {tab[atual++] = valor;}

int PilhaInt::desempilha () {
    if (atual < 0) throw;
    return tab[--atual];}

int main() {

    PilhaInt p;

    for (int i=0; i<5; i++){ p.empilha(i); cout << "empilhou" << endl;}

    while (true) { p.desempilha(); cout << "desempilhou" << endl;}

    return 0;
}
