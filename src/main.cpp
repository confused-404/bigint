#include <iostream>
#include "BigInt.h"

int main() {
    // Test constructors
    BigInt a("123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");
    BigInt c("-123456789012345678901234567890");
    BigInt d("0");
    BigInt e(1234567890, false);
    BigInt f(987654321, true);

    // Test toString
    std::cout << "a: " << a.toString() << std::endl;
    std::cout << "b: " << b.toString() << std::endl;
    std::cout << "c: " << c.toString() << std::endl;
    std::cout << "d: " << d.toString() << std::endl;
    std::cout << "e: " << e.toString() << std::endl;
    std::cout << "f: " << f.toString() << std::endl;

    // Test addition
    std::cout << "a + b: " << (a + b).toString() << std::endl;
    std::cout << "a + c: " << (a + c).toString() << std::endl;
    std::cout << "a + d: " << (a + d).toString() << std::endl;

    // Test subtraction
    std::cout << "a - b: " << (a - b).toString() << std::endl;
    std::cout << "a - c: " << (a - c).toString() << std::endl;
    std::cout << "a - d: " << (a - d).toString() << std::endl;

    // Test multiplication
    std::cout << "a * b: " << (a * b).toString() << std::endl;
    std::cout << "a * c: " << (a * c).toString() << std::endl;
    std::cout << "a * d: " << (a * d).toString() << std::endl;

    // Test division
    std::cout << "b / a: " << (b / a).toString() << std::endl;
    std::cout << "a / c: " << (a / c).toString() << std::endl;
    std::cout << "b / d: ";
    try {
        std::cout << (b / d).toString() << std::endl;
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    // Test modulus
    std::cout << "b % a: " << (b % a).toString() << std::endl;
    std::cout << "a % c: " << (a % c).toString() << std::endl;
    std::cout << "b % d: ";
    try {
        std::cout << (b % d).toString() << std::endl;
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    // Test power
    std::cout << "a ^ 2: " << a.pow(BigInt("2")).toString() << std::endl;
    std::cout << "b ^ 3: " << b.pow(BigInt("3")).toString() << std::endl;

    // Test gcd and lcm
    std::cout << "gcd(a, b): " << BigInt::gcd(a, b).toString() << std::endl;
    std::cout << "lcm(a, b): " << BigInt::lcm(a, b).toString() << std::endl;

    // Test isPrime
    std::cout << "a is prime: " << a.isPrime() << std::endl;
    std::cout << "BigInt(7) is prime: " << BigInt("7").isPrime() << std::endl;

    return 0;
}