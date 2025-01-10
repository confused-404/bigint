#ifndef BIGINT_H
#define BIGINT_H

#include <string>
#include <algorithm>
#include <iostream>
#include <utility>

class BigInt {
private:
    int* digits;
    int size;
    bool isNegative;

    std::pair<BigInt, BigInt> alldivision(const BigInt& other) const;
public:
    BigInt(std::string og);
    BigInt(int* digits, int size, bool isNegative);
    BigInt(const BigInt &other);
    BigInt(int size);
    
    ~BigInt();

    BigInt copy() const;
    BigInt abs() const;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;

    BigInt operator-() const;

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt& operator=(const BigInt &other);

    std::string toString() const;
};

#endif