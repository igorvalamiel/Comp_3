#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class PilhaInt {
    public:
        PilhaInt(int tamanho=10) {
            cap = tamanho;
            vec.resize(cap * sizeof(int));

            // ver como tratar esse erro aqui com vector
            //if (!arr) { perror("malloc");}

            for (int v = 10; v <= (10*cap); v += 10) {
                if (tam == cap) {
                    cap *= 2;
                    vec.resize(cap * sizeof(int));

                    // verificar esse erro com vector
                    //if (!tmp) { perror("realloc"); free(arr); }
                }
                vec[tam++] = v;
            }
        }

        //PilhaInt(const PilhaInt& p) {
        //    cap = p.cap;
        //    atual = p.atual;
        //    
        //    vec.resize(cap * sizeof(int));
        //    // verificar esse erro com vector
        //    //if (!arr) { perror("malloc falhou na copia"); }
        //    
        //    for (int i = 0; i < atual; i++) {
        //        vec[i] = p.vec[i];
        //    }
        //}

        //virtual ~PilhaInt() {
        //    if (vec != NULL) {
        //        free(vec);
        //    }
        //}

        const int capacidade() {
            return cap;
        }

        void redimensiona(int n) {
            
            if (atual > n) { atual = n; }
            cap = n;
            try {
                vec.resize(n * sizeof(int));
            } catch (runtime_error) {
                perror("redimenciona falhou");
            }

            for (int i = 10; i < n; i++)
                vec[i] = i;
        }

        void empilha( const int valor ){
            if (atual >= cap) {
                redimensiona(cap * 2);
            }
            vec[atual++] = valor;}
        
        const int desempilha() {
            if (atual < 0) throw;
            return vec[--atual];}

        void print( std::ostream& o, const char* msg="") {
            std::ostringstream txt;
            txt << "[ ";
            for (int i = 0; i < atual; ++i) {
                txt << vec[i];
                if (i < atual - 1) {txt << ", ";}
            }
            txt << " ]";
            std::string texto = txt.str();
            o << texto;}
        
        
        const PilhaInt& operator = ( const PilhaInt& p ){
            if( this != &p ) {
                atual = p.atual;
                cap = p.cap;

                vec.resize(cap * sizeof(int));

                for( int i = 0; i < atual; i++ ) {
                    vec[i] = p.vec[i];}
            } return p;}
        
        PilhaInt& operator << (const int valor) {
            empilha( valor );
            return *this;}
        
    private:
        int atual {0};
        int tam {0}, cap;
        vector<int> vec;
};

// função auxiliar
PilhaInt embaralha( PilhaInt q ) {
    int aux = q.desempilha();
    q << 32 << 9 << aux;
    
    return q;
}
 
int main() {
    
    PilhaInt a{3};
    for( int i = 0; i < 20; i++ ) {
    a << i;
    cout << a.capacidade() << " ";
    }
    cout << endl;
    a.print( cout ); cout << endl;

    return 0;
}
