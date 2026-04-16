#include <initializer_list>
#include <iostream>

using namespace std;

class AbstractPair {
  public:
    auto imprime (auto x, auto y) {
      return x << " = " << y;
    }
    virtual ~AbstractPair() {}
  private:
};

template <typename A, typename B>
class ImplPair : public AbstractPair {
  public:
  private:
    A a;
    B b;
};

class Pair {
  public:
    template <typename A, typename B>
    Pair( A a, B b ) {
      valor = p->imprime(a, b);
    }

    string get_value() {
      return valor;
    }

  private:
    AbstractPair *p;
    string valor;
};

void print( ostream& o, initializer_list<Pair> lista ) {
  for (auto i : lista) {
    o << i.get_value();
  }
}

int main() {
 
    Pair p( "1", 2 );
  
    print( cout, { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );

  return 0;  
}
