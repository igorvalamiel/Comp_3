using namespace std;

class AbstractPair {
  public:
    virtual void imprime(ostream& o) const = 0;

    virtual unique_ptr<AbstractPair> clone() const = 0;

    virtual ~AbstractPair() = default;
};

template <typename A, typename B>
class ImplPair : public AbstractPair {
  public:
    ImplPair(const A& x, const B& y) : a(x), b(y) {}

    void imprime(ostream& o) const override {
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
    Pair(const A& a, const B& b) {
      using DA = decay_t<A>;
      using DB = decay_t<B>;
      p = make_unique<ImplPair<DA, DB>>(a, b);
    }

    Pair(const Pair& other) {
      if (other.p) {
        p = other.p->clone();
      }
    }

    Pair& operator=(const Pair& other) {
      if (this != &other) {
        p = other.p ? other.p->clone() : nullptr;
      }
      return *this;
    }

    Pair(Pair&&) = default;
    Pair& operator=(Pair&&) = default;

    void imprime_pair(ostream& o) const {
      if (p) p->imprime(o);
    }

  private:
    unique_ptr<AbstractPair> p;
};

void print(ostream& o, initializer_list<Pair> lista) {
  for (const auto& i : lista) {
    i.imprime_pair(o);
  }
}
