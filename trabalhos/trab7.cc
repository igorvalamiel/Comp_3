using namespace std;

class AbstractPair {
  public:
    virtual void imprime (ostream& o) const = 0;
    virtual unique_ptr<AbstractPair> clone() const = 0;
    virtual ~AbstractPair() {}
  private:
};

template <typename A, typename B>
class ImplPair : public AbstractPair {
  public:

    template <typename A2, typename B2>
    ImplPair(A2&& x, B2&& y) : a(forward<A2>(x)), b(forward<B2>(y)) {}

    ImplPair(const ImplPair&) = default;

    void imprime (ostream& o) const override {
        o << a << " = " << b;
    }

    unique_ptr<AbstractPair> clone() const override {
        return make_unique<ImplPair<A, B>>(*this);
    }

  private:
    A a;
    B b;
};

struct keepObject {
    vector<unique_ptr<AbstractPair>> items;

    ~keepObject() {
        while (!items.empty()) {
            items.pop_back();
        }
    }
};

class Pair {
  public:
    template <typename A, typename B>
    Pair(A&& a, B&& b) {
        using AA = typename decay<A>::type;
        using BB = typename decay<B>::type;

        if constexpr (is_lvalue_reference<B>::value) {
            keepList = get_pool();
            auto temp = make_unique<ImplPair<AA, BB>>(forward<A>(a), b);
            p = temp->clone();
            keepList->items.push_back(move(temp));
        } else {
            p = make_unique<ImplPair<AA, BB>>(forward<A>(a), b);
        }
    }

    Pair(const Pair& other) {
        keepList = other.keepList;
        if (other.p) p = other.p->clone();
    }

    Pair& operator=(const Pair& other) {
        if (this != &other) {
            keepList = other.keepList;
            p = other.p ? other.p->clone() : nullptr;
        }

        return *this;
    }

    Pair(Pair&&) = delete;
    Pair& operator=(Pair&&) = delete;

    void imprime_pair(ostream& o) const {
        p->imprime(o);
    }

  private:
    shared_ptr<keepObject> keepList;
    unique_ptr<AbstractPair> p;

    static weak_ptr<keepObject>& current_pool() {
        static weak_ptr<keepObject> wp;
        return wp;
    }

    static shared_ptr<keepObject> get_pool() {
        auto sp = current_pool().lock();

        if (!sp) {
            sp = make_shared<keepObject>();
            current_pool() = sp;
        }

        return sp;
    }
};

void print( ostream& o, initializer_list<Pair> lista ) {
  for (const auto& i : lista) {
    i.imprime_pair(o);
    o << "\n";
  }
}
