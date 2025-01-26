#ifndef BIGINT_H
#define BIGINT_H

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

    static const int MAX_SIEVE_SIZE = 1000000;
    static bool primes[MAX_SIEVE_SIZE];

    std::pair<BigInt, BigInt> alldivision(const BigInt& other) const;
    BigInt shiftLeft(int places) const;
    void removeLeadingZeroes();
public:
    BigInt(std::string og);
    BigInt(int* digits, int size, bool isNegative);
    BigInt(const BigInt &other);
    BigInt(int size);
    BigInt(int value, bool isNegative);
    
    ~BigInt();

    BigInt copy() const;
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

    BigInt& operator=(const BigInt &other);

    bool isPrime() const;

    std::string toString() const;
    int toInt() const;
};

#endif