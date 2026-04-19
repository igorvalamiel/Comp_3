#include <iostream>
#include <vector>
using namespace std;

template <int n, typename T>
class Vetor {
    public:
        Vetor() {
            cout << "vetor construido" << endl;
        }

        virtual ~Vetor(){}

        const Vetor& operator = (initializer_list<T> valores){
            int i = 0;
            for (auto item : valores){
                vetor_v[i] = item;
                i++;
            };
            return *this;
        };

        // soma
        const Vetor operator + (const Vetor outro) const {
            Vetor resultado;

            for (int i=0; i<n; i++){
                resultado.vetor_v[i] = this->vetor_v[i] + outro.vetor_v[i];
            }

            return resultado;
        }

        //subtação
        const Vetor operator - (const Vetor outro) const {
            Vetor resultado;

            for (int i=0; i<n; i++){
                resultado.vetor_v[i] = this->vetor_v[i] - outro.vetor_v[i];
            }

            return resultado;
        }

        // produto vetor*vetor
        int operator * (const Vetor outro) const {
            auto x = 0;

            for (int i=0; i<n; i++){
                x += this->vetor_v[i] * outro.vetor_v[i];
            }

            return x;
        }

        // produto vetor*double
        const Vetor operator * (const double num) const {
            Vetor resultado;

            for (int i=0; i<n; i++){
                resultado.vetor_v[i] = this->vetor_v[i] * num;
            }

            return resultado;
        }

        // isso aqui deixa o programa ler com o auto i : vetor
        const T* begin() const { return &vetor_v[0]; }
        const T* end() const { return &vetor_v[n]; }

    private:
        int dim = n; //dimensões do vetor
        T vetor_v[n]; //criando vetor em si
};

template <int N, typename T, template<int, typename> class Estrutura>
ostream& operator << (ostream& o, const Estrutura<N,T>& col) {
    for(auto x : col)
        o << x << " ";
    return o;
};

int main( int argc, char* argv[]) {
    Vetor< 3, double> a, b;
    
    int caso;
    double x, y, z;
    
    cin >> caso >> x >> y >> z;
    a = { x, y, z };
    b = { 1, 2, 3 };
    
    switch( caso ) {
        case 1: cout << a << endl; break;
        case 2: cout << a + b << endl; break;
        /*
        case 3: {
            Vetor< 3, double> c;
            c = a ** b; 
            cout << c << endl;
            break; 
        }*/
        case 4: cout << a * b << endl; break;
        case 5: cout << a * 3.0 << endl; break;
        //case 6: cout << 2.1 * a << endl; break;
        case 7: cout << a + b + a * 2.0 << endl; break;
    }
  
  return 0;
}