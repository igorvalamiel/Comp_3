#include <initializer_list>
#include <iostream>

using namespace std;

class Pair {
public:
  template <typename A, typename B>
  Pair( A a, B b ) {
    // código aqui...
  }
private:
  AbstractPair *p;
};

class AbstractPair {

};

void print( ostream& o, initializer_list<Pair> lista ) {}

int main() {
 
    Pair p( "1", 2 );
  
    print( cout, { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );

  return 0;  
}
