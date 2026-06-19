#include <iostream>
#include <vector>
#include "trab19_var.cc"

using namespace std;

int main() {

    cout << (Var(0) == Var("A")) << endl;
    cout << (Var(36) == Var("A")) << endl;
    cout << (Var('A') == Var("A")) << endl;

    return 0;
}
