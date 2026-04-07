#include <iostream>
#include <sstream>

using namespace std;

class PilhaInt {
    public:
        PilhaInt(int tamanho=10) {
            cap = tamanho;
            
            arr = (int*) malloc( cap * sizeof(int) );
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
        }

        PilhaInt(const PilhaInt& p) {
            cap = p.cap;
            atual = p.atual;
            
            arr = (int*) malloc(cap * sizeof(int));
            if (!arr) { perror("malloc falhou na copia"); }
            
            for (int i = 0; i < atual; i++) {
                arr[i] = p.arr[i];
            }
        }

        virtual ~PilhaInt() {
            if (arr != NULL) {
                free(arr);
            }
        }

        const int capacidade() {
            return cap;
        }

        void redimensiona(int n) {
            
            if (atual > n) { atual = n; }
            cap = n;
            int* tmp = (int*) realloc( arr, n * sizeof(int) );

            if (tmp == NULL) {
                perror("realloc falhou");
                free(arr);
            }
            arr = tmp;

            for (int i = 10; i < n; i++)
                arr[i] = i;
        }

        void empilha( const int valor ){
            if (atual >= cap) {
                redimensiona(cap * 2);
            }
            arr[atual++] = valor;}
        
        const int desempilha() {
            if (atual < 0) throw;
            return arr[--atual];}

        void print( std::ostream& o, const char* msg="") {
            std::ostringstream txt;
            txt << "[ ";
            for (int i = 0; i < atual; ++i) {
                txt << arr[i];
                if (i < atual - 1) {txt << ", ";}
            }
            txt << " ]";
            std::string texto = txt.str();
            o << texto;}
        
        //const PilhaInt& copy( const PilhaInt& p ){}
        //virtual ~copy(){}
        
        const PilhaInt& operator = ( const PilhaInt& p ){
            if( this != &p ) {
                atual = p.atual;
                cap = p.cap;

                int* tmp = (int*) realloc(arr, cap * sizeof(int));
                if (tmp != NULL) {
                    arr = tmp;
                }

                for( int i = 0; i < atual; i++ ) {
                    arr[i] = p.arr[i];}
            } return p;}
        
        PilhaInt& operator << (const int valor) {
            empilha( valor );
            return *this;}

        // função auxiliar
        PilhaInt embaralha( PilhaInt q ) {
            int aux = q.desempilha();
            q << 32 << 9 << aux;
            
            return q;
        }
        
    private:
        int atual {0};
        int tam {0}, cap;
        int* arr;
};
 
int main() {
    
    return 0;
}
