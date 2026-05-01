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
  using DA = decay_t<A>;
  using DB = decay_t<B>;
  public:
    ImplPair (A&& x, B&& y) : a(forward<A>(x)), b(forward<B>(y)) {}

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

    virtual ~Pair() {}

  private:
    unique_ptr<AbstractPair> p;
};

void print( ostream& o, const initializer_list<Pair>& lista ) {
  for (auto& i : lista) {
    i.imprime_pair(o);
  }
}