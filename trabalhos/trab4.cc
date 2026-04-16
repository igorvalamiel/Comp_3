#include <initializer_list>
#include <iostream>

using namespace std;

class AbstractPair {
  public:
    virtual string imprime () {}
    virtual ~AbstractPair() {}
  private:
};

template <typename A, typename B>
class ImplPair : public AbstractPair {
  public:
    string imprime () {
      return a << " = " << b;    
    }
  private:
    A a;
    B b;
};

class Pair {
  public:
    template <typename A, typename B>
    Pair( A a, B b ) {
      p = new ImplPair<A, B>(a, b);
    }

    string imprime_pair() {
      return p->imprime();
    }

  private:
    AbstractPair *p;
    string valor;
};

void print( ostream& o, initializer_list<Pair> lista ) {
  for (auto i : lista) {
    o << i.imprime_pair();
  }
}

int main() {
 
    Pair p( "1", 2 );
  
    print( cout, { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );

  return 0;  
}
