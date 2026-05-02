using namespace std;


template <typename L1, typename L2>
void operator | (L1 lado1, L2 lado2) {
    for_each( lado1, lado1+10,  lado2 );
}
