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
            }
        }

        ostream get_values(){
            ostream s;
            for (auto i : vetor_v){
                s << i << " ";
            }
            return s;
        }

    private:
        int dim = n; //dimensões do vetor
        T vetor_v[n]; //criando vetor em si
};

// ajustando o operador <<
template <int n, typename T>
std::ostream& operator<<(std::ostream& o, Vetor<n, T>& v) {
    
    o << v.get_values();
    
    return o;
}

int main( int argc, char* argv[]) {
    Vetor< 3, double> a, b;
    
    int caso;
    double x, y, z;
    
    cin >> caso >> x >> y >> z;
    a = { x, y, z };
    b = { 1, 2, 3 };

    cout << b << endl;
    
    /*
    switch( caso ) {
        case 1: cout << a << endl; break;
        case 2: cout << a + b << endl; break;
        case 3: {
            Vetor< 3, double> c;
            c = a ** b; 
            cout << c << endl;
            break; 
        }
        case 4: cout << a * b << endl; break;
        case 5: cout << a * 3.0 << endl; break;
        case 6: cout << 2.1 * a << endl; break;
        case 7: cout << a + b + a * 2.0 << endl; break;
    }
    */
  
  return 0;
}