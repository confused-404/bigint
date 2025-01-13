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

BigInt::BigInt(int size)
{
    this->size = size;
    this->isNegative = 0;
    this->digits = new int[size]();
}

BigInt::BigInt(int value, bool isNegative)
{
    this->size = 0;
    int temp = value;
    while (temp > 0)
    {
        temp /= 1000;
        this->size++;
    }

    this->isNegative = isNegative;
    this->digits = new int[this->size]();
    for (int d = 0; d < this->size; d++)
    {
        this->digits[d] = value % 1000;
        value /= 1000;
    }
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
        newbigint.removeLeadingZeroes();
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

        int diff = thisdigit - otherdigit - borrow;

        if (diff < 0)
        {
            diff += 1000;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        newdigits[d] = diff;
    }

    int resultSize = maxSize;
    while (resultSize > 0 && newdigits[resultSize - 1] == 0)
        resultSize--;

    if (resultSize == 0)
    {
        delete[] newdigits;
        return BigInt("0");
    }

    BigInt result = BigInt(newdigits, resultSize, false);
    result.removeLeadingZeroes();
    return result;
}

BigInt BigInt::operator*(const BigInt &other) const
{
    int maxSize = other.size + this->size;
    int *results = new int[maxSize]();

    for (int d = 0; d < maxSize; d++)
    {
        for (int t = 0; t <= d; t++)
        {
            int o = d - t;
            if (o < other.size && t < this->size)
            {
                results[d] += this->digits[t] * other.digits[o];
                if (results[d] > 1000000000)
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
    result.removeLeadingZeroes();
    return result;
}

BigInt BigInt::operator/(const BigInt &other) const
{
    return this->alldivision(other).first;
}

BigInt BigInt::operator%(const BigInt &other) const
{
    return this->alldivision(other).second;
}

BigInt BigInt::pow(const BigInt &other) const
{
    // std::cout << "pow called with a = " << this->toString() << ", and b = " << other.toString() << std::endl;

    if (other == BigInt("0"))
        return BigInt("1");
    if (*this == BigInt("0"))
        return BigInt("0");
    if (other < BigInt("0"))
        throw std::invalid_argument("Negative exponents not supported");

    BigInt result("1");
    BigInt binarypig(other);
    BigInt accumulator(*this);

    while (binarypig > BigInt("0"))
    {
        // std::cout << "result: " << result.toString() << std::endl;

        if (binarypig.digits[0] % 2 == 1)
        {
            result = result * accumulator;
            // std::cout << "Updated result: " << result.toString() << std::endl;
        }

        accumulator = accumulator * accumulator;
        // std::cout << "Squared accumulator: " << accumulator.toString() << std::endl;

        // std::cout << "binarypig before division: " << binarypig.toString() << std::endl;
        // if (binarypig == BigInt("1")) std::cout << "binaryipg is 1" << std::endl;
        binarypig = binarypig / BigInt("2");
        // std::cout << "Reduced binarypig: " << binarypig.toString() << std::endl;
    }

    // std::cout << "result: " << result.toString() << std::endl;
    return result;
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
    if (this->size == 0)
        return "0";

    std::string fin = "";
    if (this->isNegative)
        fin.append("-");

    fin.append(std::to_string(this->digits[this->size - 1]));

    for (int d = this->size - 2; d >= 0; d--)
    {
        fin.append(std::string(3 - std::to_string(this->digits[d]).length(), '0'));
        fin.append(std::to_string(this->digits[d]));
    }
    return fin;
}

std::pair<BigInt, BigInt> BigInt::alldivision(const BigInt &other) const
{
    if (*this < other)
        return std::make_pair(BigInt("0"), *this);
    if (other == BigInt("0"))
        throw std::runtime_error("Division by 0");
    if (other == BigInt("1"))
        return std::make_pair(*this, BigInt("0"));

    BigInt remainder = *this;
    BigInt quotient(this->size + 1);
    quotient.digits = new int[this->size + 1]();

    for (int i = this->size - 1; i >= other.size - 1; i--)
    {
        long long sigdividend = remainder.digits[i];
        if (i + 1 < remainder.size)
        {
            sigdividend += static_cast<long long>(remainder.digits[i + 1]) * 1000;
        }
        if (i + 2 < remainder.size)
        {
            sigdividend += static_cast<long long>(remainder.digits[i + 2]) * 1000000;
        }

        int sigdivisor = other.digits[other.size - 1];
        int result = sigdividend / sigdivisor;
        BigInt bigresult(result, false);

        // std::cout << "result of " << sigdividend << " / " << sigdivisor << " = " << result << std::endl;

        // std::cout << "other: " << other.toString() << std::endl;
        // std::cout << "result: " << bigresult.toString() << std::endl;
        // std::cout << "other * result = " << (other * bigresult).toString() << std::endl;
        BigInt shiftedDivisor = (other * bigresult).shiftLeft(i - (other.size - 1));
        // while (shiftedDivisor > remainder)
        // {
        //     if (bigresult == BigInt("0"))
        //     {
        //         std::cout << "bigresult reached zero, breaking loop." << std::endl;
        //         break;
        //     }

        //     // std::cout << "while shifteddivisor > reemainder iteration" << std::endl;
        //     std::cout << "shiftedDivisor: " << shiftedDivisor.toString() << std::endl;
        //     std::cout << "remainder: " << remainder.toString() << std::endl;
        //     std::cout << "Before decrement: bigresult = " << bigresult.toString() << std::endl;
        //     bigresult = bigresult - 1;
        //     std::cout << "After decrement: bigresult = " << bigresult.toString() << std::endl;

        //     shiftedDivisor = (other * bigresult).shiftLeft(i - (other.size - 1));
        // }
        while (shiftedDivisor > remainder)
        {
            // std::cout << "Before decrement: bigresult = " << bigresult.toString() << std::endl;

            BigInt oldBigResult = bigresult;
            bigresult = bigresult - 1;

            // std::cout << "After decrement: bigresult = " << bigresult.toString() << std::endl;
            if (bigresult == oldBigResult)
            {
                // std::cout << "bigresult is not decrementing, breaking to avoid infinite loop." << std::endl;
                break;
            }

            shiftedDivisor = (other * bigresult).shiftLeft(i - (other.size - 1));
            // std::cout << "Updated shiftedDivisor: " << shiftedDivisor.toString() << std::endl;
            // std::cout << "Remainder: " << remainder.toString() << std::endl;

            if (bigresult == BigInt("0"))
            {
                // std::cout << "bigresult reached zero, breaking loop." << std::endl;
                break;
            }
        }

        // std::cout << "remainder = " << remainder.toString() << " - " << shiftedDivisor.toString();
        remainder = remainder - shiftedDivisor;
        // std::cout << " = " << remainder.toString() << std::endl;

        quotient.digits[i - (other.size - 1)] = bigresult.toInt();
    }

    quotient.removeLeadingZeroes();
    remainder.removeLeadingZeroes();
    return std::make_pair(quotient, remainder);
}

BigInt BigInt::shiftLeft(int places) const
{
    // std::cout << "shiftLeft of " << this->toString() << " with " << places << " places" << std::endl;
    if (places <= 0)
        return *this;
    BigInt result(*this);
    result.size += places;

    int *newDigits = new int[result.size]();
    for (int i = 0; i < this->size; i++)
    {
        newDigits[i + places] = this->digits[i];
    }
    delete[] result.digits;
    result.digits = newDigits;
    return result;
}

int BigInt::toInt() const
{
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

void BigInt::removeLeadingZeroes() {
    int newsize = this->size;
    for (int d = 0; d < this->size; d++) {
        if (this->digits[this->size - 1 - d] == 0) {
            newsize--;
        }
    }
    this->size = newsize;
}