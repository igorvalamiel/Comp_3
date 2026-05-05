#include <initializer_list>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Leak {
    public:
        Leak(): n( ++contador ) {
            cout << "Criou: " << n << endl;
        }
        
        Leak( const Leak& p ): n( ++contador ) {
            cout << "Criou: " << n << " copiando de " << p.n << endl;
        }
        
        ~Leak() {
            cout << "Destruiu: " << n << endl;
        }
        
        ostream& print( ostream& o ) const {
            return o << "lk(" << n << ")";
        }
        
    private:
        static int contador;
        int n;
};

int Leak::contador = 0;

ostream& operator << ( ostream& o, const Leak& v ) {
    return v.print( o ); 
}

template <typename T>
ostream& operator << ( ostream& o, const vector<T>& v ) {
    o << "[ ";
    for( auto& x : v )
      o << x << " ";
      
    return o << "]";
}

class AbstractPair {
  public:
    virtual void imprime (ostream& o) const {}
    virtual unique_ptr<AbstractPair> clone() const = 0;
    virtual ~AbstractPair() {}
  private:
};

template <typename A, typename B>
class ImplPair : public AbstractPair {
  using DA = decay_t<A>;
  using DB = decay_t<B>;
  public:
    ImplPair(const DA& x, const DB& y) : a(x), b(y) {}

    virtual ~ImplPair() {}

    void imprime (ostream& o) const override {
      o << a << " = " << b << endl;    
    }

    unique_ptr<AbstractPair> clone() const override {
      return make_unique<ImplPair<DA, DB>>(a, b);
    }

  private:
    DA a;
    DB b;
};

class Pair {
  public:
    template <typename A, typename B>
    Pair(A&& a, B&& b) {
        p = make_unique<ImplPair<A, B>>(forward<A>(a), forward<B>(b));
    }

    void imprime_pair(ostream& o) const {
      return p->imprime(o);
    }

    Pair(const Pair& other) {
      p = other.p->clone();
    }

    Pair(Pair&& other) {
      p = other.p->clone();
    }

    virtual ~Pair() {}

  private:
    unique_ptr<AbstractPair> p;
};

void print( ostream& o, const initializer_list<Pair>& lista ) {
  for (auto& i : lista) {
    i.imprime_pair(o);
  }
}

int main() {

    cout << "==========================================================================" << endl;
    print( cout, { { "jan", Leak() }, { string( "pi" ), Leak() } } );
    cout << "==========================================================================" << endl;
    Leak *l = new Leak[2];
    print( cout, { { "jan", l[0] }, { string( "pi" ), l[1] } } );
    delete [] l;
    cout << "==========================================================================" << endl;
    shared_ptr<Leak> l2 ( new Leak() );
    print( cout, { { "jan", *l2 }, { string( "pi" ), *l2 } } );
    cout << "==========================================================================" << endl;
    Leak a, b, c;
    vector<Leak> v = { a, b };
    print( cout, { { "v", v }, { "c", c } } );
    cout << "==========================================================================" << endl;

  return 0;  
}