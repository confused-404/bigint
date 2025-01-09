#include "BigInt.h"

int main() {
    BigInt a = BigInt("7329810758490326509263489058");
    // BigInt a = BigInt("23");
    // std::cout << "a.toString(): " << a.toString() << std::endl;
    BigInt b = BigInt("6785490238710987328906542890907780975243");
    // BigInt b = BigInt("472");
    // std::cout << "b.toString(): " << b.toString() << std::endl;

    BigInt sum = (a + b);
    std::cout << "a + b = " << sum.toString() << std::endl;
    std::cout << "a - b = " << (a - b).toString() << std::endl;
    std::cout << "b - a = " << (b - a).toString() << std::endl;
}