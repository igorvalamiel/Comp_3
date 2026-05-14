#include <algorithm>

using namespace std;


template <typename I, typename F>
void operator | (I& item, F funcao) {
    for_each( begin(item), end(item),  funcao );
}
