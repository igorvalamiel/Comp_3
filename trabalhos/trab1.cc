#include <iostream>
#include <sstream>

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
        int atual;
};
 
int main() {
    PilhaInt p, q;
    q << 2;
    p << 19 << 18 << 17 << 30;
    q = p;
    p.desempilha();
    q << 7;
    stringstream ssp, ssq;
    p.print( ssp );
    q.print( ssq );

    cout << "q = " << ssq.str() << "\n" << "p = " << ssp.str() << endl;

    // Essa linha é apenas para gerar um erro se o "operator=" não for definido. Ignore-a!
    auto l = &PilhaInt::operator=; (p.*l)(q);

    return 0;
}