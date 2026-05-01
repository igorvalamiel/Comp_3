using namespace std;

class AbstractPair {
  public:
    virtual void imprime (ostream& o) const {}
    virtual unique_ptr<AbstractPair> clone() const = 0;
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

    void imprime (ostream& o) const override {
      o << a << " = " << b << endl;    
    }

    unique_ptr<AbstractPair> clone() const override {
      return make_unique<ImplPair<A, B>>(a, b);
    }

  private:
    A a;
    B b;
};

class Pair {
  public:
    template <typename A, typename B>
    Pair( A a, B b ) {
      p = make_unique<ImplPair<A, B>>(a, b);
    }

    Pair( const Pair& other) {
      p = other.p->clone();
    }

    void imprime_pair(ostream& o) const {
      return p->imprime(o);
    }

    virtual ~Pair() {}

  private:
    unique_ptr<AbstractPair> p;
};

void print( ostream& o, initializer_list<Pair> lista ) {
  for (auto& i : lista) {
    i.imprime_pair(o);
  }
}