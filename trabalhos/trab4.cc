#include <initializer_list>
#include <iostream>

using namespace std;

class Pair {
public:
  template <typename A, typename B>
  Pair( A a, B b ) {

  }

  string print (auto s1, auto s2) {
    return s1 + " = " + s2 + "\n";
  }

private:
  //AbstractPair *p;
  
};

template <typename A, typename B>
class AbstractPair {
public:
    AbstractPair (A a, B b){
        s1 = a;
        s2 = b;
    }
private:
    A s1;
    B s2;
}

void print( ostream& o, initializer_list<Pair> lista ) {
    for (auto i : lista) {
        string txt = i.print();
        o << txt;
    }
}

int main() {
 
    Pair p( "1", 2 );
  
    print( cout, { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );

  return 0;  
}