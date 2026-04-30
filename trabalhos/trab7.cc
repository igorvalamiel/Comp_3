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
    ImplPair(A&& x, B&& y) : a(forward<A>(x)), b(forward<B>(y)) {}

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
    Pair(A&& a, B&& b) {
      p = make_unique<ImplPair<A, B>>(forward<A>(a), forward<B>(b));
    }

    void imprime_pair(ostream& o) const {
      return p->imprime(o);
    }

  private:
    unique_ptr<AbstractPair> p;
};

void print( ostream& o, initializer_list<Pair> lista ) {
  for (const auto& i : lista) {
    i.imprime_pair(o);
  }
}
