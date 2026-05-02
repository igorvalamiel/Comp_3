using namespace std;


template <typename L1, typename L2>
void operator | (L1 lado1, L2 lado2) {
    for_each( tab, tab+10,  []( int x ) { cout << x*x << endl; } );
}
