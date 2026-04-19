#include <iostream>
using namespace std;

template <typename n, typename T>
class Vetor : public vector<int, T> {

};

int main( int argc, char* argv[]) {
    Vetor< 3, double> a, b;
    /*
    int caso;
    double x, y, z;
    
    cin >> caso >> x >> y >> z;
    a = { x, y, z };
    b = { 1, 2, 3 };
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