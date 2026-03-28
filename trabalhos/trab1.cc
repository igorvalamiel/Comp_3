#include <iostream>
//#include <sstream>

using namespace std;

class PilhaInt {
    public:
        PilhaInt(): atual(0) {}

        void empilha( const int valor ){
            tab[atual++] = valor;}
        
        const int desempilha() {
            if (atual < 0) throw;
            return tab[--atual];}

        void print( std::ostream& o, const char* msg="") {
            o << msg << std::endl;}
        
        PilhaInt operator << (const int valor) {
            empilha( valor );
            return *this;}
        
    private:
        static const int MAX_PILHA = 10;
        int tab[MAX_PILHA];
        int atual;
};
 
int main() {
    PilhaInt p;
    p << 19 << 18 << 17 << 30;
    p.print( cout );
    cout << endl;

    return 0;
}