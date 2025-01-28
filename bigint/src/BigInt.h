#pragma once

#include <string>
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>

class BigInt {
private:
    int* digits;
    int size;
    bool isNegative;

    std::pair<BigInt, BigInt> alldivision(const BigInt& other) const;
    static void calculateCarry(int& n, int& carry);
    BigInt shiftLeft(int places) const;
    void normalize();
public:
    BigInt(std::string og);
    BigInt(const BigInt& other);
    BigInt(int n);
    BigInt(int* digits, int size, bool isNegative);

    ~BigInt();

    BigInt abs() const;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;
    BigInt pow(const BigInt& other) const;

    BigInt operator-() const;

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt& operator=(const BigInt& other);

    static BigInt gcd(const BigInt& a, const BigInt& b);
    static BigInt lcm(const BigInt& a, const BigInt& b);
    bool isPrime() const;

    std::string toString() const;
    int toInt() const;
};