#include <iostream>
#include <sstream>

using namespace std;

class PilhaInt {
    public:
        PilhaInt(int tamanho=10) {
            cap = tamanho;
            
            int* arr = (int*) malloc( cap * sizeof(int) );
            if (!arr) { perror("malloc");}

            for (int v = 10; v <= (10*cap); v += 10) {
                if (tam == cap) {
                    cap *= 2;
                    int* tmp = (int*) realloc( arr, cap * sizeof(int) );
                    if (!tmp) { perror("realloc"); free(arr); }
                    arr = tmp;
                }
                arr[tam++] = v;
            }

            free(arr);
            arr = NULL;
        }

        const int capacidade() {
            return cap;
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
        int tam {0}, cap;
};
 
int main() {
    
    PilhaInt a(5), b(15);
    cout << a.capacidade() << endl;
    cout << b.capacidade() << endl;

    return 0;
}
