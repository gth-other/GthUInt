/*
 *  GthUInt
 *  Copyright (C) 2023 gth-other
 *
 *  GthUInt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GthUInt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GthUInt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GthUInt.hpp"


GthUInt::GthUInt() : GthUInt(0) {}
GthUInt::GthUInt(uint64_t number) {
    if (number < BASE) {
        this->data.push_back(number);
    }
    else {
        this->data.resize(2);
        this->data[0] = number % BASE;
        this->data[1] = number / BASE;
    }
}
GthUInt::GthUInt(const std::string &string) {
    if (string.empty()) {
        this->data.push_back(0);
        return;
    }

    this->data.reserve(string.size() / BASE_LENGTH);
    for (auto i = (int32_t)string.length(); i > 0; i = i - BASE_LENGTH) {
        if (i < BASE_LENGTH) {
            this->data.push_back(std::stoull(string.substr(0, i)));
        }
        else {
            this->data.push_back(std::stoull(string.substr(i - BASE_LENGTH, BASE_LENGTH)));
        }
    }

    this->removeLeadingZeroes();
}
std::ostream &operator<<(std::ostream &ostream, const GthUInt &a) {
    ostream << a.data.back();

    char oldFill = ostream.fill();
    ostream.fill('0');
    for (int32_t i = (int32_t)a.data.size() - 2; i >= 0; i = i - 1) {
        ostream << std::setw(GthUInt::BASE_LENGTH) << a.data[i];
    }
    ostream.fill(oldFill);

    return ostream;
}
bool operator==(const GthUInt &a, const GthUInt &b) {
    return (GthUInt::compare(a, b) == GthUInt::CompareFlags::Equal);
}
bool operator!=(const GthUInt &a, const GthUInt &b) {
    return (GthUInt::compare(a, b) != GthUInt::CompareFlags::Equal);
}
bool operator>(const GthUInt &a, const GthUInt &b) {
    return (GthUInt::compare(a, b) == GthUInt::CompareFlags::Bigger);
}
bool operator>=(const GthUInt &a, const GthUInt &b) {
    return (GthUInt::compare(a, b) != GthUInt::CompareFlags::Less);
}
bool operator<(const GthUInt &a, const GthUInt &b) {
    return (GthUInt::compare(a, b) == GthUInt::CompareFlags::Less);
}
bool operator<=(const GthUInt &a, const GthUInt &b) {
    return (GthUInt::compare(a, b) != GthUInt::CompareFlags::Bigger);
}
GthUInt operator+(GthUInt a, const GthUInt &b) {
    if (b.data.size() > a.data.size()) {
        a.data.reserve(b.data.size() - a.data.size());
    }

    bool carry = false;
    for (int32_t i = 0; i < std::max(a.data.size(), b.data.size()) or carry; i = i + 1) {
        if (i == a.data.size()) {
            a.data.push_back(0);
        }
        uint128_t sum = (uint128_t)a.data[i] + (uint128_t)(i < b.data.size() ? b.data[i] : 0) + (uint128_t)carry;
        carry = (sum >= GthUInt::BASE);
        if (carry) {
            a.data[i] = sum - GthUInt::BASE;
        }
        else {
            a.data[i] = sum;
        }
        if (i >= b.data.size() and !carry) {
            break;
        }
    }

    return a;
}
GthUInt operator-(GthUInt a, const GthUInt &b) {
    bool carry = false;
    for (int32_t i = 0; i < b.data.size() or carry; i = i + 1) {
        int128_t difference = (int128_t)a.data[i] - (int128_t)(i < b.data.size() ? b.data[i] : 0) - (int128_t)carry;
        carry = (difference < 0);
        if (carry) {
            a.data[i] = difference + GthUInt::BASE;
        }
        else {
            a.data[i] = difference;
        }
    }
    a.removeLeadingZeroes();
    return a;
}
GthUInt operator*(const GthUInt &a, const GthUInt &b) {
    GthUInt c;
    c.data.resize(a.data.size() + b.data.size());
    for (int32_t i = 0; i < a.data.size(); i = i + 1) {
        uint64_t carry = 0;
        for (int32_t j = 0; j < b.data.size() or carry; j = j + 1) {
            uint128_t product = (uint128_t)a.data[i] * (uint128_t)(j < b.data.size() ? b.data[j] : 0) + (uint128_t)c.data[i + j] + (uint128_t)carry;
            c.data[i + j] = product % GthUInt::BASE;
            carry = product / GthUInt::BASE;
        }
    }
    c.removeLeadingZeroes();
    return c;
}
std::pair<GthUInt, GthUInt> GthUInt::divmod(const GthUInt &a, const GthUInt &b) {
    if (b.data.size() == 1) {
        return divmodShort(a, b.data[0]);
    }
    return divmodLong(a, b);
}
GthUInt GthUInt::pow(GthUInt a, uint64_t b) {
    GthUInt result = 1;
    while (b) {
        if (b % 2 == 0) {
            b = b / 2;
            a = a * a;
        }
        else {
            b = b - 1;
            result = result * a;
        }
    }
    return result;
}
GthUInt GthUInt::factorial(uint64_t a) {
    GthUInt result = 1;
    for (uint64_t i = 2; i <= a; i = i + 1) {
        result = result * i;
    }
    return result;
}
bool GthUInt::even() const {
    return (this->data[0] % 2 == 0);
}
bool GthUInt::odd() const {
    return !this->even();
}
std::string GthUInt::getString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
}
void GthUInt::removeLeadingZeroes() {
    while (this->data.back() == 0 and this->data.size() > 1) {
        this->data.pop_back();
    }
}
void GthUInt::rightShift() {
    if (this->data.empty()) {
        this->data.push_back(0);
        return;
    }

    this->data.push_back(this->data.back());
    for (int32_t i = (int32_t)this->data.size() - 2; i > 0; i = i - 1) {
        this->data[i] = this->data[i - 1];
    }
    this->data[0] = 0;
}
uint8_t GthUInt::compare(const GthUInt &a, const GthUInt &b) {
    if (a.data.size() > b.data.size()) {
        return Bigger;
    }
    if (a.data.size() < b.data.size()) {
        return Less;
    }
    for (int32_t i = (int32_t)a.data.size() - 1; i >= 0; i = i - 1) {
        if (a.data[i] > b.data[i]) {
            return Bigger;
        }
        if (a.data[i] < b.data[i]) {
            return Less;
        }
    }
    return Equal;
}
std::pair<GthUInt, GthUInt> GthUInt::divmodLong(const GthUInt &a, const GthUInt &b) {
    GthUInt result, current;
    result.data.resize(a.data.size());
    current.data.resize(0);

    for (int32_t i = (int32_t)a.data.size() - 1; i >= 0; i = i - 1) {
        current.rightShift();
        current.data[0] = a.data[i];
        current.removeLeadingZeroes();

        uint64_t x = 0, l = 0, r = GthUInt::BASE;
        while (l <= r) {
            uint64_t m = ((uint128_t)l + (uint128_t)r) / (uint128_t)2;
            if (b * m <= current) {
                x = m;
                l = m + 1;
            }
            else {
                r = m - 1;
            }
        }

        result.data[i] = x;
        current = current - b * x;
    }

    result.removeLeadingZeroes();
    return std::make_pair(result, current);
}
std::pair<GthUInt, uint64_t> GthUInt::divmodShort(GthUInt a, uint64_t b) {
    uint64_t carry = 0;
    for (int32_t i = (int32_t)a.data.size() - 1; i >= 0; i = i - 1) {
        uint128_t product = (uint128_t)carry * (uint128_t)BASE + (uint128_t)a.data[i];
        a.data[i] = product / b;
        carry = product % b;
    }
    a.removeLeadingZeroes();
    return std::make_pair(a, carry);
}