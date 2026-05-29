#include <iostream>

using namespace std;

template <typename F, typename I = void>
class Bind {
    public:
        Bind(F f, I i) : Func(f), item(i) {}

        template <typename... Args>
        auto operator()(Args... args) {
            return Func(item, args...);
        }
        
    private:
        F Func;
        I item;
};

template <typename F>
class Bind<F, void> {
    public:

        Bind(F f) : Func(f) {}

        template <typename... Args>
        auto operator()(Args... args) {
            return Func(args...);
        }
        
    private:
        F Func;
};

template <typename F, typename I>
Bind<F,I> bind(const F Func, const I item) {
    return Bind<F,I>(Func, item);
}

template <typename F>
Bind<F> bind(F Func) {
    return Bind<F>(Func);
}