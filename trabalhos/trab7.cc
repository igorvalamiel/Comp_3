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
    template <typename A2, typename B2>
    ImplPair(A2&& x, B2&& y) : a(forward<A2>(x)), b(forward<B2>(y)) {}

    ImplPair(const A& x, const B& y) : a(x), b(y) {}

    void imprime (ostream& o) const override {
      o << a << " = " << b << endl;    
    }

    unique_ptr<AbstractPair> clone() const override {
      auto tmpA = a;
      auto tmpB = b;
      return make_unique<ImplPair<decay_t<A>, decay_t<B>>>(tmpA, tmpB);
    }

  private:
    A a;
    B b;
};

class Pair {
  public:
    template <typename A, typename B>
    Pair(A&& a, B&& b) {
        using DA = decay_t<A>;
        using DB = decay_t<B>;

        DA a_copy = forward<A>(a);
        DB b_copy = forward<B>(b);

        p = make_unique<ImplPair<DA, DB>>(a_copy, b_copy);
    }

    Pair(const Pair& other) {
        p = other.p->clone();
    }

    Pair(Pair&& other) = delete;

    void imprime_pair(ostream& o) const {
        p->imprime(o);
    }

  private:
    unique_ptr<AbstractPair> p;
};

void print( ostream& o, const initializer_list<Pair>& lista ) {
  for (const auto& i : lista) {
    i.imprime_pair(o);
  }
}