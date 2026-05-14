#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <string.h>

using namespace std;

#include "trab9_stream.cc"

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

int main() {

    int v1[] = { 2, 9, 8, 8, 7, 4 };
    auto result = v1 | []( int x ) { return x % 2 == 0; };
    for( auto x : result ) 
        cout << x << " ";

    return 0;
}