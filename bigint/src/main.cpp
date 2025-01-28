#include <iostream>
#include <cassert>
#include "BigInt.h"

void test_constructor() {
    std::cout << "Testing constructor" << std::endl;
    BigInt a("123456789012345678901234567890");
    assert(a.toString() == "123456789012345678901234567890");

    BigInt b("-987654321098765432109876543210");
    assert(b.toString() == "-987654321098765432109876543210");

    BigInt c("1234567890");
    assert(c.toString() == "1234567890");

    BigInt d("-987654321");
    assert(d.toString() == "-987654321");

    BigInt e("0");
    assert(e.toString() == "0");
    std::cout << "Constructor test passed!" << std::endl;
}

void test_addition() {
    std::cout << "Testing addition" << std::endl;
    BigInt a("123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");
    BigInt result = a + b;
    assert(result.toString() == "1111111110111111111011111111100");

    BigInt c("-123456789012345678901234567890");
    result = a + c;
    assert(result.toString() == "0");

    BigInt d("0");
    result = a + d;
    assert(result.toString() == "123456789012345678901234567890");
    std::cout << "Addition test passed!" << std::endl;
}

void test_subtraction() {
    std::cout << "Testing subtraction" << std::endl;
    BigInt a("123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");
    BigInt d("0");

    BigInt result = a - d;
    // std::cout << "result of subtraction a-d: " << result.toString() << std::endl;
    assert(result.toString() == "123456789012345678901234567890");

    BigInt c("-123456789012345678901234567890");
    result = a - c;
    // std::cout << "result of subtraction a-c: " << result.toString() << std::endl;
    assert(result.toString() == "246913578024691357802469135780");

    result = a - b;
    // std::cout << "result of subtraction a-b: " << result.toString() << std::endl;
    assert(result.toString() == "-864197532086419753208641975320");
    std::cout << "Subtraction test passed!" << std::endl;
}

void test_multiplication() {
    std::cout << "Testing multiplication" << std::endl;
    BigInt a("123456789");
    BigInt b("987654321");
    BigInt result = a * b;
    assert(result.toString() == "121932631112635269");

    BigInt c("-123456789");
    result = a * c;
    assert(result.toString() == "-15241578750190521");

    BigInt d("0");
    result = a * d;
    assert(result.toString() == "0");
    std::cout << "Multiplication test passed!" << std::endl;
}

void test_division() {
    std::cout << "Testing division" << std::endl;
    BigInt a("1000");
    BigInt b("10");
    BigInt result = a / b;
    assert(result.toString() == "100");

    a = BigInt("1001");
    result = a / b;
    assert(result.toString() == "100");

    a = BigInt("10");
    b = BigInt("100");
    result = a / b;
    assert(result.toString() == "0");

    a = BigInt("123456789");
    b = BigInt("1");
    result = a / b;
    assert(result.toString() == "123456789");

    a = BigInt("3217987804573");
    b = BigInt("8979826137");
    result = a / b;
    assert(result.toString() == "358");

    a = BigInt("123456789");
    b = BigInt("0");
    try {
        result = a / b;
        assert(false); // Should not reach here
    }
    catch (const std::runtime_error& e)  {
        // assert(std::string(e.what()) == "Division by 0");
    }
    std::cout << "Division test passed!" << std::endl;
}

void test_modulus() {
    std::cout << "Testing modulus" << std::endl;
    BigInt a("1000");
    BigInt b("10");
    BigInt result = a % b;
    assert(result.toString() == "0");

    a = BigInt("1001");
    result = a % b;
    assert(result.toString() == "1");

    a = BigInt("10");
    b = BigInt("100");
    result = a % b;
    assert(result.toString() == "10");

    a = BigInt("123456789");
    b = BigInt("1");
    result = a % b;
    assert(result.toString() == "0");

    a = BigInt("3217987804573");
    b = BigInt("8979826137");
    result = a % b;
    assert(result.toString() == "3210047527");

    a = BigInt("123456789");
    b = BigInt("0");
    try {
        result = a % b;
        assert(false); // Should not reach here
    }
    catch (const std::runtime_error& e) {
        // assert(std::string(e.what()) == "Division by 0");
    }
    std::cout << "Modulus test passed!" << std::endl;
}

void test_utility_functions() {
    std::cout << "Testing utility functions" << std::endl;
    BigInt a("123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");

    std::cout << "Testing abs()" << std::endl;
    assert(a.abs().toString() == "123456789012345678901234567890");
    assert(b.abs().toString() == "987654321098765432109876543210");

    BigInt c("-123456789012345678901234567890");
    assert(c.abs().toString() == "123456789012345678901234567890");

    BigInt d("0");
    assert(d.abs().toString() == "0");

    std::cout << "Testing gcd()" << std::endl;
    assert(BigInt::gcd(a, b).toString() == "9000000000900000000090");

    std::cout << "Testing lcm()" << std::endl;
    assert(BigInt::lcm(a, b).toString() == "13548070124980948012498094801236261410");

    std::cout << "Testing isPrime()" << std::endl;
    assert(a.isPrime() == false);
    assert(BigInt("2").isPrime() == true);
    assert(BigInt("17").isPrime() == true);
    assert(BigInt("18").isPrime() == false);
    // assert(BigInt("180252380737439").isPrime() == true);
    // assert(BigInt("28871271685161").isPrime() == false);
    assert(BigInt("50728129").isPrime() == true);
    assert(BigInt("50728121").isPrime() == false);

    std::cout << "Utility functions test passed!" << std::endl;
}

int main() {
    test_constructor();
    test_addition();
    test_subtraction();
    test_multiplication();
    test_division();
    test_modulus();
    test_utility_functions();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}