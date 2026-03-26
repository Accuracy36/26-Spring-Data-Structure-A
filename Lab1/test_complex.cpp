#include <iostream>
#include "complex_number.cpp"

int main() {
    Complex a("3+4.0i");
    Complex b(1.0, -4.0);
    Complex c = a + b;
    c = a.add(b);
    c.print_complex();
    return 0;
}