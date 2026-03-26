#include <iostream>
#include <string>
#include "big_int.cpp"

int main() {
    // initialize BigInt from string
    BigInt a("-100000");
    std::cout << "a = " << a.toString() << std::endl;
    // initialize BigInt from int
    BigInt b(100000);
    std::cout << "b = " << b.toString() << std::endl;
    BigInt c(-52345);
    BigInt d("12346");
    std::cout << "abs"<< c.compareAbsolute(d) << std::endl;
    std::cout << "compare"<< c.compare(d) << std::endl;
    std::cout << "a + b = " << (a + b).toString() << std::endl;
    std::cout << "a - b = " << (a - b).toString() << std::endl;
    std::cout << "a * b = " << (a * b).toString() << std::endl;
    // test 
    return 0;
}