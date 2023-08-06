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


#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstdint>


#pragma once


typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;


class GthUInt {
public:
    GthUInt();
    GthUInt(uint64_t number);
    GthUInt(const std::string &string);

    friend std::ostream& operator <<(std::ostream& ostream, const GthUInt& a);

    friend bool operator ==(const GthUInt &a, const GthUInt &b);
    friend bool operator !=(const GthUInt &a, const GthUInt &b);
    friend bool operator >(const GthUInt &a, const GthUInt &b);
    friend bool operator >=(const GthUInt &a, const GthUInt &b);
    friend bool operator <(const GthUInt &a, const GthUInt &b);
    friend bool operator <=(const GthUInt &a, const GthUInt &b);

    friend GthUInt operator+(GthUInt a, const GthUInt &b);
    friend GthUInt operator-(GthUInt a, const GthUInt &b);
    friend GthUInt operator*(const GthUInt &a, const GthUInt &b);
    static std::pair<GthUInt, GthUInt> divmod(const GthUInt &a, const GthUInt &b);

    static GthUInt pow(GthUInt a, uint64_t b);
    static GthUInt factorial(uint64_t a);
    [[nodiscard]] bool even() const;
    [[nodiscard]] bool odd() const;
    [[nodiscard]] std::string getString() const;
private:
    std::vector<uint64_t> data;

    static constexpr uint64_t BASE = 10000000000000000000u;
    static constexpr uint32_t BASE_LENGTH = 19;
    enum CompareFlags {
        Equal, Bigger, Less
    };

    void removeLeadingZeroes();
    void rightShift();
    static uint8_t compare(const GthUInt &a, const GthUInt &b);
    static std::pair<GthUInt, GthUInt> divmodLong(const GthUInt &a, const GthUInt &b);
    static std::pair<GthUInt, uint64_t> divmodShort(GthUInt a, uint64_t b);
};