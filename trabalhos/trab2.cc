#include <iostream>
#include <sstream>

using namespace std;

class PilhaInt {
    public:
        PilhaInt(int tamanho=10) {
            tam = tamanho;
        }

        const int get_tam() {
            return tam;
        }

        void empilha( const int valor ){
            tab[atual++] = valor;}
        
        const int desempilha() {
            if (atual < 0) throw;
            return tab[--atual];}

        void print( std::ostream& o, const char* msg="") {
            std::ostringstream txt;
            txt << "[ ";
            for (int i = 0; i < atual; ++i) {
                txt << tab[i];
                if (i < atual - 1) {txt << ", ";}
            }
            txt << " ]";
            std::string texto = txt.str();
            o << texto;}
        
        const PilhaInt& operator = ( const PilhaInt& p ){
            if( this != &p ) {
                atual = p.atual;
                for( int i = 0; i < p.atual; i++ ) {
                    tab[i] = p.tab[i];}
            } 
            return p;}
        
        PilhaInt& operator << (const int valor) {
            empilha( valor );
            return *this;}
        
    private:
        static const int MAX_PILHA = 10;
        int tab[MAX_PILHA];
        int atual {0};
        int tam;
};
 
int main() {
    
    PilhaInt p;
    p.empilha( 1 );
    p << 3 << 9 << 13 << 89;
    cout << p.desempilha() << endl;
    cout << p.desempilha() << endl;
    p.print( cout );
    p << 19 << 18 << 17 << 30;
    stringstream ss;
    p.print( ss );
    cout << endl << "{" << ss.str() << "}" << endl;

    return 0;
}
