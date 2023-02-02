/// @file value.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @copyright (c) 2014-2023 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#pragma once

#include <ginac/ginac.h>
#include <ostream>
#include <string>

namespace symsolbin
{

/// A value.
class value_t {
public:
    /// @brief Construct a new value_t object.
    /// @param name the name of the variable.
    value_t(std::string name, double value = .0, bool replace = false);

    inline value_t &set_value(double value)
    {
        _value   = value;
        _replace = true;
        return *this;
    }

    inline double get_value() const
    {
        return _value;
    }

    inline value_t &set_replace(bool replace)
    {
        _replace = replace;
        return *this;
    }

    inline bool get_replace() const
    {
        return _replace;
    }

    inline friend GiNaC::ex operator*(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs * rhs._value : lhs * rhs._symbol;
    }

    inline friend GiNaC::ex operator*(const value_t &lhs, const GiNaC::ex &rhs)
    {
        return (lhs._replace) ? lhs._value * rhs : lhs._symbol * rhs;
    }

    inline friend GiNaC::ex operator/(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs / rhs._value : lhs / rhs._symbol;
    }

    inline friend GiNaC::ex operator/(const value_t &lhs, const GiNaC::ex &rhs)
    {
        return (lhs._replace) ? lhs._value / rhs : lhs._symbol / rhs;
    }

    inline friend GiNaC::ex operator+(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs + rhs._value : lhs + rhs._symbol;
    }

    inline friend GiNaC::ex operator+(const value_t &lhs, const GiNaC::ex &rhs)
    {
        return (lhs._replace) ? lhs._value + rhs : lhs._symbol + rhs;
    }

    inline friend GiNaC::ex operator-(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs - rhs._value : lhs - rhs._symbol;
    }

    inline friend GiNaC::ex operator-(const value_t &lhs, const GiNaC::ex &rhs)
    {
        return (lhs._replace) ? lhs._value - rhs : lhs._symbol - rhs;
    }

    inline friend GiNaC::ex operator-(const value_t &lhs)
    {
        return -(lhs._replace) ? GiNaC::ex(lhs._value) : GiNaC::ex(lhs._symbol);
    }

    inline friend GiNaC::ex operator==(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs == rhs._value : lhs == rhs._symbol;
    }

    inline friend bool operator==(const value_t &lhs, const value_t &rhs)
    {
        return lhs._symbol == rhs._symbol;
    }

    inline friend std::ostream &operator<<(std::ostream &lhs, const value_t &rhs)
    {
        lhs << rhs._symbol;
        return lhs;
    }

private:
    GiNaC::symbol _symbol;
    double _value;
    bool _replace;
};

/// A list of values.
using value_list_t = std::vector<value_t>;

bool collection_contains_value(const value_list_t &collection, const value_t &value);

std::ostream &operator<<(std::ostream &lhs, const value_list_t &rhs);

} // namespace symsolbin