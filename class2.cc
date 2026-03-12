#include <iostream>

using namespace std;

const int MAX_PILHA = 10;

class PilhaInt {
    public:
        PilhaInt();

        virtual ~PilhaInt() {
            cout << "Chamou o destrutor" << endl;
        }

        virtual void empilha(int valor) {
            tab[atual++] = valor;
        }

        int desempilha () {
            return tab[--atual];
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
}

int main() {

    PilhaInt p;

    p.empilha(9);

    return 0;
}