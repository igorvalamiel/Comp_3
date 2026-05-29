#include <iostream>
#include <algorithm>
#include <array>
#include "trab14_bind.cc"
using namespace std;

long mdc( long a, long b ) { return b == 0 ? a : mdc( b, a%b ); }

struct MMC {
  auto operator()( long a, long b ) { return a*b/mdc(a,b); }
};

struct BarraPesada {
  template <typename A, typename B>
  auto operator()( A a, B b ) { return a + b; }
};

string ordena( string a, string b, string c, string d,
               string e, string f, string g, string h,
               string i, string j, string k, string l ) {
  std::array<string, 12> tab = { a,b,c,d,e,f,g,h,i,j,k,l };
  string result;
  std::sort( tab.begin(), tab.end() );
  for( auto itr = tab.begin(); itr != tab.end(); ++itr )
    result += *itr + " ";
  return result;
}

int main() {
  using ::bind;   // usa nossa bind, não std::bind
  
  auto f1 = bind( mdc, 12 );
    for( int i = 2; i <= 12; i++ ) cout << f1( i ) <<  " ";

  return 0;
}