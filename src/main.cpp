#include "BigInt.h"

int main()
{
    BigInt a = BigInt("54263723901");
    // BigInt a = BigInt("23");
    // // std::cout << "a.toString(): " << a.toString() << std::endl;
    BigInt b = BigInt("78964371896");
    // BigInt b = BigInt("47");
    // // std::cout << "b.toString(): " << b.toString() << std::endl;

    // std::cout << "a + b = " << (a + b).toString() << std::endl;
    // std::cout << "a - b = " << (a - b).toString() << std::endl;
    // std::cout << "b - a = " << (b - a).toString() << std::endl;
    // std::cout << "a * b = " << (a * b).toString() << std::endl;
    // std::cout << "a / b = " << (a / b).toString() << std::endl;
    // std::cout << "b / a = " << (b / a).toString() << std::endl;
    // std::cout << "a mod b = " << (a % b).toString() << std::endl;
    // std::cout << "b mod a = " << (b % a).toString() << std::endl;
    std::cout << "a ^ b = " << (a.pow(b)).toString() << std::endl;
    std::cout << "b ^ a = " << (b.pow(a)).toString() << std::endl;
}