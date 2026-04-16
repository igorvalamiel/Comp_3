#include <initializer_list>
#include <iostream>

using namespace std;

class AbstractPair {
  public:
    virtual void imprime (ostream& o) {}
    virtual ~AbstractPair() {}
  private:
};

template <typename A, typename B>
class ImplPair : public AbstractPair {
  public:
    ImplPair (A x, B y){
      a = x;
      b = y;
    }

    virtual ~ImplPair() {}

    void imprime (ostream& o) {
      o << a << " = " << b << endl;    
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

    void imprime_pair(ostream& o) {
      return p->imprime(o);
    }

  private:
    AbstractPair *p;
    string valor;
};

void print( ostream& o, initializer_list<Pair> lista ) {
  for (auto i : lista) {
    i.imprime_pair(o);
  }
}

int main() {
 
    Pair p( "1", 2 );
  
    print( cout, { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );

  return 0;  
}
