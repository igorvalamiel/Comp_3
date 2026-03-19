#include <iostream>

using namespace std;

template <class Etype>
inline const Etype& Max (
    const Etype& a,
    const Etype& b ) {
  return a > b ? a : b;
}

template <typename Tipo>
class Pilha {
    public:
        Pilha(): atual(0) {}

        void empilha( const Tipo& valor ){
            tab[atual++] = valor;}
        
        const Tipo& desempilha() {
            if (atual < 0) throw;
            return tab[--atual];}
        
        Pilha<Tipo>& operator << (const Tipo& valor) {
            empilha( valor );
            return *this;}
        
    private:
        static const int MAX_PILHA = 10;
        Tipo tab[MAX_PILHA];
        int atual;
};


int main() {
    return 0;
}
