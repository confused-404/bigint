#pragma once

#include "BigInt.h"

template class std::vector<bool>;

bool BigInt::primesCalculated = false;
std::vector<char> BigInt::primes(BigInt::STORED_PRIMES, 1);

void BigInt::initializePrimes() {
    std::cout << "Initializing primes" << std::endl;
    if (primesCalculated) return;
    primes[0] = primes[1] = false;

    int sqrtsize = std::sqrt(BigInt::STORED_PRIMES);

    for (int i = 4; i < BigInt::STORED_PRIMES; i += 2) {
        primes[i] = false;
    }
    for (int i = 3; i < sqrtsize; i++) {
        if (primes[i] && (long long) i * i <= BigInt::STORED_PRIMES)
        if (primes[i]) {
            for (int j = i * i; j < BigInt::STORED_PRIMES; j += i) {
                primes[j] = false;
            }
        }
    }
    primesCalculated = true;
    std::cout << "Done initializing primes" << std::endl;
}

std::pair<BigInt, BigInt> BigInt::alldivision(const BigInt& other) const {
    if (*this < other)
        return std::make_pair(BigInt(0), *this);
    if (other == BigInt(0))
        throw std::runtime_error("Division by 0");
    if (other == BigInt(1))
        return std::make_pair(*this, BigInt(0));

    BigInt remainder(*this);
    BigInt quotient(*this);
    quotient.digits = new int[this->size + 1]();
    //std::cout << "Starting alldivision. this: " << this->toString() << ", other: " << other.toString() << std::endl;

    for (int i = this->size - 1; i >= other.size - 1; i--) {
        long long sigdividend = remainder.digits[i];
        if (i + 1 < remainder.size) {
            sigdividend += static_cast<long long>(remainder.digits[i + 1]) * 1000;
        }
        if (i + 2 < remainder.size) {
            sigdividend += static_cast<long long>(remainder.digits[i + 2]) * 1000000;
        }

        int sigdivisor = other.digits[other.size - 1];
        if (sigdivisor == 0) {
            std::cerr << "Division by 0 in alldivision loop detected" << std::endl;
            throw std::runtime_error("Division by 0 detected in significant digits");
        }

        int result = sigdividend / sigdivisor;
        //std::cout << "sigdividend: " << sigdividend << ", sigdivisor: " << sigdivisor << ", result: " << result << std::endl;

        BigInt bigresult(result);
        BigInt shiftedDivisor = (other * bigresult).shiftLeft(i - (other.size - 1));

        while (shiftedDivisor > remainder) {
            bigresult = bigresult - 1;
            shiftedDivisor = (other * bigresult).shiftLeft(i - (other.size - 1));
        }

        remainder = remainder - shiftedDivisor;
        quotient.digits[i - (other.size - 1)] = bigresult.toInt();

        //std::cout << "Loop iteration " << i << ": remainder = " << remainder.toString()
        //    << ", quotient = " << quotient.toString() << std::endl;
    }

    quotient.normalize();
    remainder.normalize();
    //std::cout << "Final quotient: " << quotient.toString() << ", remainder: " << remainder.toString() << std::endl;
    return std::make_pair(quotient, remainder);
}


void BigInt::calculateCarry(int& n, int& carry) {
    const long DIGIT_BOUND = 0x3E8;

    if (n < 0)
    {
        carry = ((-n) + DIGIT_BOUND - 1) / DIGIT_BOUND;
        n += carry * DIGIT_BOUND;
    }
    else
    {
        carry = (n / DIGIT_BOUND);
        n %= DIGIT_BOUND;
    }
}

BigInt BigInt::shiftLeft(int places) const {
    if (places <= 0)
        return *this;

    BigInt result(*this);
    int newSize = this->size + places;

    int* newDigits = new int[newSize]();
    for (int i = 0; i < this->size; i++) {
        newDigits[i + places] = this->digits[i];
    }

    delete[] result.digits;
    result.digits = newDigits;
    result.size = newSize;

    return result;
}


void BigInt::normalize() {
    while (this->size > 1 && this->digits[this->size - 1] == 0) {
        this->size--;
    }
    if (this->size < 0) {
        throw std::runtime_error("Normalize detected negative size");
    }
    if (this->size == 0 || (this->size == 1 && this->digits[0] == 0)) {
        this->size = 1;
        this->digits[0] = 0;
        this->isNegative = false;
    }
}


BigInt::BigInt(std::string og) {
    int s = og.size();

    if (s < 1) return;

    this->isNegative = og.at(0) == '-';
    this->size = (s - this->isNegative) / 3 + ((s - this->isNegative) % 3 != 0);

    this->digits = new int[this->size]();

    int start, length;
    int digitIndex = 0;
    for (int i = s; i > this->isNegative; i -= 3)
    {
        start = std::max((int)this->isNegative, i - 3);
        length = i - start;

        if (start < 0 || start >= s || length <= 0)
            throw std::invalid_argument("Invalid start or length in string parsing");

        this->digits[digitIndex++] = stoi(og.substr(start, length));
    }
    this->normalize();
}

BigInt::BigInt(const BigInt& other) {
    this->size = other.size;
    this->isNegative = other.isNegative;
    this->digits = new int[this->size]();
    std::copy(other.digits, other.digits + this->size, this->digits);
    this->normalize();
}

BigInt::BigInt(int n) {
    if (n == 0) {
        this->size = 1;
        this->digits = new int[1]();
        this->isNegative = false;
    }
    else {

        this->size = 0;
        int temp = n;
        while (temp > 0)
        {
            temp /= 1000;
            this->size++;
        }

        this->isNegative = n < 0;
        this->digits = new int[this->size]();
        for (int d = 0; d < this->size; d++)
        {
            this->digits[d] = n % 1000;
            n /= 1000;
        }
        this->normalize();
    }
}

BigInt::BigInt(int* digitsToUse, int sizeToUse, bool isNegativeToUse) {
    this->size = sizeToUse;
    this->isNegative = isNegativeToUse;
    this->digits = new int[this->size]();
    std::copy(digitsToUse, digitsToUse + sizeToUse, this->digits);
    this->normalize();
}

BigInt::~BigInt() {
    if (this->digits == nullptr && this->size > 0) {
        throw std::runtime_error("Heap corruption: digits is nullptr but size > 0");
    }
    if (this->digits != nullptr) {
        std::cout << "Deleting a BigInt with digits: " << this->digits << ", size: " << this->size << ", and address: " << this << std::endl;
        delete[] this->digits;
        this->digits = nullptr;
    }
    this->size = 0;
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (this->isNegative && other.isNegative)
    {
        BigInt result = this->abs() + other.abs();
        result.isNegative = true;
        result.normalize();
        return result;
    }
    if (!this->isNegative && !other.isNegative)
    {
        int maxSize = std::max(this->size, other.size);
        int* newdigits = new int[maxSize + 1]();

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
        newbigint.normalize();
        return newbigint;
    }

    if (this->isNegative)
    {
        BigInt result = other - this->abs();
        result.normalize();
        return result;
    }
    else
    {
        BigInt result = *this - other.abs();
        result.normalize();
        return result;
    }
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (*this < other)
    {
        return -(other - *this);
    }
    if (*this == other)
    {
        return BigInt("0");
    }
    if (this->isNegative && other.isNegative)
    {
        return other.abs() - this->abs();
    }
    if (other.isNegative)
    {
        return *this + other.abs();
    }

    int* resultDigits = new int[this->size]();
    int carry = 0;

    for (int i = 0; i < other.size; i++)
    {
        int n = this->digits[i] - other.digits[i] - carry;
        BigInt::calculateCarry(n, carry);
        resultDigits[i] = n;
    }

    for (int i = other.size; i < this->size; i++)
    {
        if (this->digits[i] < carry)
        {
            int n = this->digits[i] - carry;
            BigInt::calculateCarry(n, carry);
            resultDigits[i] = n;
        }
        else
        {
            resultDigits[i] = this->digits[i] - carry;
            carry = 0;
        }
    }

    BigInt result = BigInt(resultDigits, this->size, false);

    result.normalize();

    return result;
}

BigInt BigInt::operator*(const BigInt& other) const {
    int maxSize = other.size + this->size;
    int* results = new int[maxSize]();

    for (int d = 0; d < maxSize; d++)
    {
        for (int t = 0; t <= d; t++)
        {
            int o = d - t;
            if (o < other.size && t < this->size)
            {
                results[d] += this->digits[t] * other.digits[o];
                if (results[d] > 1000)
                {
                    results[d + 1] += results[d] / 1000;
                    results[d] %= 1000;
                }
            }
        }
        if (results[d] > 999)
        {
            results[d + 1] += results[d] / 1000;
            results[d] %= 1000;
        }
    }

    BigInt result = BigInt(results, maxSize - (results[maxSize - 1] == 0), this->isNegative ^ other.isNegative);
    result.normalize();
    delete[] results;
    return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
    return this->alldivision(other).first;
}

BigInt BigInt::operator%(const BigInt& other) const {
    return this->alldivision(other).second;
}

BigInt BigInt::pow(const BigInt& other) const {
    if (other == BigInt(0))
        return BigInt(1);
    if (*this == BigInt(0))
        return BigInt(0);
    if (other < BigInt(0))
        return BigInt(0);

    BigInt result("1");
    BigInt base = *this;
    BigInt exp = other;

    while (exp > BigInt("0"))
    {
        if (exp.digits[0] % 2 == 1)
        {
            result = result * base;
        }
        base = base * base;
        exp = exp / BigInt("2");
    }
    result.normalize();
    return result;
}

BigInt BigInt::operator-() const {
    BigInt temp(*this);
    temp.isNegative = !temp.isNegative;
    return temp;
}

bool BigInt::operator==(const BigInt& other) const {
    if (this->size == 1 && this->digits[0] == 0 && other.size == 1 && other.digits[0] == 0)
        return true;
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

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    return (*this != other) && !(*this > other);
}

bool BigInt::operator<=(const BigInt& other) const {
    return !(*this > other);
}

bool BigInt::operator>(const BigInt& other) const {
    if (!this->isNegative && other.isNegative)
        return true;
    if (this->isNegative && !other.isNegative)
        return false;

    bool isNegative = this->isNegative;

    if (this->size > other.size)
        return !isNegative;
    if (other.size > this->size)
        return isNegative;

    for (int d = this->size - 1; d >= 0; d--)
    {
        if (this->digits[d] > other.digits[d])
            return !isNegative;
        if (this->digits[d] < other.digits[d])
            return isNegative;
    }

    return false;
}

bool BigInt::operator>=(const BigInt& other) const {
    return !(*this < other);
}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other)
    {
        delete[] this->digits;
        this->size = other.size;
        this->isNegative = other.isNegative;
        this->digits = new int[this->size]();
        std::copy(other.digits, other.digits + this->size, this->digits);
    }
    return *this;
}

BigInt BigInt::abs() const {
    BigInt temp(*this);
    temp.isNegative = false;
    return temp;
}

BigInt BigInt::gcd(const BigInt& a, const BigInt& b) {
    BigInt x = a.abs();
    BigInt y = b.abs();

    while (y != BigInt(0))
    {
        BigInt temp = y;
        y = x % y;
        x = temp;
    }
    x.normalize();
    return x;
}

BigInt BigInt::lcm(const BigInt& a, const BigInt& b) {
    if (a == BigInt(1))
        return b;
    if (b == BigInt(1))
        return a;
    return a * b / BigInt::gcd(a, b);
}

bool BigInt::isPrime() const {
    if (*this <= BigInt(BigInt::STORED_PRIMES)) {
        BigInt::initializePrimes();
        return BigInt::primes[this->toInt()];
    }

    BigInt n(*this);

    if (n % BigInt(2) == BigInt(0))
        return n == BigInt(2);

    for (BigInt testNum = BigInt(3); testNum * testNum <= n; testNum = testNum + BigInt(2))
    {
        if (n % testNum == BigInt(0))
            return false;
    }

    return true;
}

std::string BigInt::toString() const {
    if (this->size == 0)
        return "0";

    std::string fin;
    fin.reserve(this->size * 3 + (this->isNegative ? 1 : 0));

    if (this->isNegative)
        fin.append("-");

    fin.append(std::to_string(this->digits[this->size - 1]));

    for (int d = this->size - 2; d >= 0; d--)
    {
        std::string digitStr = std::to_string(this->digits[d]);
        std::string paddedStr = std::string(3 - digitStr.length(), '0') + digitStr;

        fin.append(paddedStr);
    }

    return fin;
}

int BigInt::toInt() const {
    int result = 0;
    int multiplier = 1;

    for (int i = 0; i < this->size; i++)
    {
        if (result > (INT_MAX - this->digits[i] * multiplier) / 1000)
        {
            throw std::overflow_error("BigInt value is too large to fit in an int");
        }

        result += this->digits[i] * multiplier;
        multiplier *= 1000;
    }

    return this->isNegative ? -result : result;
}