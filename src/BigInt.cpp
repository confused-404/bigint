#include "BigInt.h"

BigInt::BigInt(std::string og)
{
    // std::cout << "Constructor with str: " << og << std::endl;
    int s = og.size();

    if (s < 1)
        return;

    this->isNegative = og.at(0) == '-';
    this->size = (s - this->isNegative) / 3 + ((s - this->isNegative) % 3 != 0);

    this->digits = new int[this->size];

    // std::cout << "Starting to iteratore" << std::endl;
    int start, length;
    for (int digit = 0; digit < this->size; digit++)
    {
        // std::cout << "iteration with digit: " << digit << std::endl;
        start = s - 3 - digit * 3;
        if (start < 0)
            start = this->isNegative;
        length = (digit == this->size - 1) ? (s - (this->size - 1) * 3) : 3;
        // std::cout << "before substr" << std::endl;
        // std::cout << "start: " << start << ", length: " << length << std::endl;
        // std::cout << "substr: " << og.substr(start, length) << " ---" << std::endl;
        this->digits[digit] = stoi(og.substr(start, length));
        // std::cout << "::after substr" << std::endl;
    }
    // std::cout << "ending iteration" << std::endl;
}

BigInt::BigInt(int *digits, int size, bool isNegative)
{

    this->digits = digits;
    this->size = size;
    this->isNegative = isNegative;
}

BigInt::BigInt(const BigInt &other)
{
    this->size = other.size;
    this->isNegative = other.isNegative;
    this->digits = new int[this->size];
    std::copy(other.digits, other.digits + this->size, this->digits);
}

BigInt::~BigInt()
{
    delete[] this->digits;
}

BigInt BigInt::copy() const
{
    BigInt copy = BigInt(this->digits, this->size, this->isNegative);
    return copy;
}

BigInt BigInt::abs() const
{
    BigInt copy = this->copy();
    copy.isNegative = false;
    return copy;
}

BigInt BigInt::operator+(const BigInt &other) const
{
    // std::cout << "Additing" << std::endl;
    if (this->isNegative && other.isNegative)
    {
        BigInt result = this->abs() + other.abs();
        result.isNegative = true;
        return result;
    }
    if (!this->isNegative && !other.isNegative)
    {
        int maxSize = std::max(this->size, other.size);
        int *newdigits = new int[maxSize + 1]();

        int carry = 0;
        for (int d = 0; d < maxSize; d++)
        {
            int thisdigit = (d < this->size) ? this->digits[d] : 0;
            int otherdigit = (d < other.size) ? other.digits[d] : 0;
            int sum = thisdigit + otherdigit + carry;
            newdigits[d] = sum % 1000;
            carry = sum / 1000;
        }

        if (carry > 0)
        {
            newdigits[maxSize] = carry;
        }

        BigInt newbigint = BigInt(newdigits, maxSize + (carry != 0), false);
        // std::cout << "newbigint: " << newbigint.toString() << std::endl;
        return newbigint;
    }

    if (this->isNegative)
    {
        return other - this->abs();
    }
    else
    {
        return *this - other.abs();
    }
}

BigInt BigInt::operator-(const BigInt &other) const
{
    if (this->isNegative && other.isNegative)
    {
        return other.abs() - this->abs();
    }

    if (this->isNegative)
    {
        return -*this + other;
    }
    if (other.isNegative)
    {
        return *this + other.abs();
    }

    if (*this < other)
    {
        BigInt result = other - *this;
        result.isNegative = true;
        return result;
    }

    int maxSize = std::max(this->size, other.size);
    int *newdigits = new int[maxSize]();

    int borrow = 0;

    for (int d = 0; d < maxSize; d++)
    {
        int thisdigit = (d < this->size) ? this->digits[d] : 0;
        int otherdigit = (d < other.size) ? other.digits[d] : 0;

        if (thisdigit < otherdigit + borrow)
        {
            thisdigit += 1000;
            newdigits[d] = thisdigit - otherdigit;
            borrow = 1;
        }
        else
        {
            newdigits[d] = thisdigit - borrow - otherdigit;
            borrow = 0;
        }
    }

    int resultSize = maxSize;
    while (resultSize > 0 && newdigits[resultSize - 1] == 0)
        resultSize--;

    BigInt result = BigInt(newdigits, resultSize, false);
    return result;
}

BigInt BigInt::operator*(const BigInt &other) const
{
}

BigInt BigInt::operator-() const
{
    BigInt copy = this->copy();
    copy.isNegative = !copy.isNegative;
    return copy;
}

bool BigInt::operator==(const BigInt &other) const
{
    if (this->isNegative != other.isNegative)
        return false;
    if (this->size != other.size)
        return false;

    for (int digit = 0; digit < this->size; digit++)
    {
        if (this->digits[digit] != other.digits[digit])
            return false;
    }

    return true;
}

bool BigInt::operator!=(const BigInt &other) const
{
    return !(*this == other);
}

bool BigInt::operator<(const BigInt &other) const
{
    return !(*this > other) && *this != other;
}

bool BigInt::operator<=(const BigInt &other) const
{
}

bool BigInt::operator>(const BigInt &other) const
{
    if (!this->isNegative && other.isNegative)
        return true;
    if (this->isNegative && !other.isNegative)
        return false;

    if (!this->isNegative)
    {
        if (this->size > other.size)
            return true;
        if (other.size > this->size)
            return false;
        for (int d = 0; d < this->size; d++)
        {
            if (this->digits[this->size - 1 - d] > other.digits[other.size - 1 - d])
                return true;
        }
    }

    if (this->isNegative)
    {
        if (this->size > other.size)
            return false;
        if (other.size > this->size)
            return true;
        for (int d = 0; d < this->size; d++)
        {
            if (this->digits[this->size - 1 - d] < other.digits[other.size - 1 - d])
                return true;
        }
    }

    return false;
}

bool BigInt::operator>=(const BigInt &other) const
{
}

BigInt &BigInt::operator=(const BigInt &other)
{
    if (this != &other)
    {
        delete[] this->digits;
        this->size = other.size;
        this->isNegative = other.isNegative;
        this->digits = new int[this->size];
        std::copy(other.digits, other.digits + this->size, this->digits);
    }
    return *this;
}

std::string BigInt::toString() const
{
    std::string fin = "";
    if (this->isNegative)
        fin.append("-");
    for (int d = 1; d <= this->size; d++)
    {
        fin.append(std::to_string(this->digits[this->size - d]));
    }
    return fin;
}