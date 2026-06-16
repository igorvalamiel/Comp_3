
#include <iostream>

using namespace std;

/*
class 
    stream anterior
    begin end
*/

class Intervalo {
    public:
    Intervalo (int value_a): begin_value(value_a), end_value(value_a-2) {}
        Intervalo (int value_a, int value_b): begin_value(value_a), end_value(value_b) {}

        class Iterador {
            public:
                Iterador (int v) : atual(v) {}

                int operator *() const { return atual; }

                int operator ++() { return ++atual;} // prefixado ( ++a )

                int operator ++(int) { return atual++;} // posfixado ( a++ )
                
                bool operator != ( Iterador i) const { return atual != i.atual; }

            private:
                int atual;
        };

        Iterador begin() {return begin_value;}

        Iterador end() { return end_value+1;}

    private:
        int begin_value, end_value;
};


int main() {
    Intervalo intervalo(4, 10);

    // for (auto x = intervalo.begin(); x != intervalo.end(); ++x)
    for (auto x : intervalo) {
        cout << x << " ";
    }
}