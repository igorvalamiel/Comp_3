#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <string.h>

using namespace std;

#include "trab8_stream.cc"

template <typename T> 
void myPrint( T t ) {
    cout << t << " ";
}

struct PrintM {
  void operator() ( vector<int> v ) {
      cout << "[ " << v[0] << ", " << v[1] << " ] ";
  }
};

struct PrintM2 {
  void operator() ( vector<int> v ) {
      v | myPrint<int>;
  }
};

string StringToUpper(string strToConvert)
{
   for (std::string::iterator p = strToConvert.begin(); strToConvert.end() != p; ++p)
       *p = toupper(*p);

   return strToConvert;
}

struct PrintM3 {
    void operator()( string st ) {
        cout << StringToUpper( st ) << " ";
    }
};

int main( int argc, char* argv[]) {
    int caso = 0;
    double v = 0;

    cin >> caso >> v;

    switch( caso ) {
        case 1: {
            vector<int> v1 = { 2, 9, 8, 8 };
            vector<string> v2 = { "jose", "casado" };
            vector<double> v3 = { 3.11, 5, 7.9 };
            
            v1 | []( int n ){ cout << n << " "; };
            cout << endl;
            v2 | []( string n ){ cout << n << " "; };
            cout << endl;
            v3 | []( double n ){ cout << n << " "; };
            cout << endl;
        break;
        }

        case 2: {
            int tabInt[] = { 1, 5, 9, 3 };
            double tabDouble[] = { 3.1, 5.3, 9 };
            string tabString[] = { "a", "e", "i", "o", "u", "ypsilon" };
            
            tabInt | []( int n ){ cout << n << " "; };
            cout << endl;
            tabDouble | []( double n ){ cout << n << " "; };
            cout << endl;
            tabString | []( string n ){ cout << n << " "; };
            cout << endl;
        break;
        }

        case 3: {
            vector<int> v1 = { 1, 9, 5, 8 };
            vector<string> v2 = { "joao", "Solteiro" };
            vector<double> v3 = { 3.14, 999, 7.9, 900 };
            
            v1 | myPrint<int>;
            cout << endl;
            v2 | myPrint<string>;
            cout << endl;
            v3 |  myPrint<double>;
            cout << endl;
        break;
        }
        
        case 4: {
            vector<vector<int>> m = { { 1, 2 }, { 4, 6 }, { 6, 7 } };
            string tab[] = { "hello,", "world" };
            
            m | PrintM();
            cout << endl;
            m | PrintM2();
            cout << endl;
            tab | PrintM3();
        break;
        }

        case 5: {
            auto x = { 1, 2, 3 };
            x | []( int x ) { cout << x*x << " "; };
        break;
        }
    }
  
  return 0;
}