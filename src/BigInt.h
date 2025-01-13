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

    std::string toString() const;
    int toInt() const;
};

#endif