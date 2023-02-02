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

/// @brief A value.
class value_t {
public:
    /// @brief Construct a new value_t object.
    /// @param name the name of the variable.
    /// @param value the numerical value of the variable.
    /// @param replace if the symbol should be replaced by the numerical value when used.
    value_t(std::string name, double value = .0, bool replace = false);

    value_t(const value_t &other) = default;

    value_t &operator=(const value_t &other) = default;

    ~value_t() = default;

    /// @brief Sets the numerical value.
    /// @param value the one we want to set.
    /// @return a reference to this value.
    inline value_t &set_value(double value)
    {
        _value   = value;
        _replace = true;
        return *this;
    }

    /// @brief Returns the stored numerical value.
    inline double get_value() const
    {
        return _value;
    }

    /// @brief If true the value is replaced with its numerical value.
    /// @param replace controls the replacement.
    /// @return a reference to this value.
    inline value_t &set_replace(bool replace)
    {
        _replace = replace;
        return *this;
    }

    /// @brief Returns the current replacement option.
    /// @return true the value is meant to be replaced by its numerical value,
    /// false if the symbol should be used instead.
    inline bool get_replace() const
    {
        return _replace;
    }

    /// @brief Multiplies an expression by the value.
    inline friend GiNaC::ex operator*(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs * rhs._value : lhs * rhs._symbol;
    }

    /// @brief Multiplies the value by an expression.
    inline friend GiNaC::ex operator*(const value_t &lhs, const GiNaC::ex &rhs)
    {
        return (lhs._replace) ? lhs._value * rhs : lhs._symbol * rhs;
    }

    /// @brief Divides an expression by the value.
    inline friend GiNaC::ex operator/(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs / rhs._value : lhs / rhs._symbol;
    }

    /// @brief Divides the value by an expression.
    inline friend GiNaC::ex operator/(const value_t &lhs, const GiNaC::ex &rhs)
    {
        return (lhs._replace) ? lhs._value / rhs : lhs._symbol / rhs;
    }

    /// @brief Sums an expression and the value.
    inline friend GiNaC::ex operator+(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs + rhs._value : lhs + rhs._symbol;
    }

    /// @brief Sums the value and an expression.
    inline friend GiNaC::ex operator+(const value_t &lhs, const GiNaC::ex &rhs)
    {
        return (lhs._replace) ? lhs._value + rhs : lhs._symbol + rhs;
    }

    /// @brief Substracts the value from an expression.
    inline friend GiNaC::ex operator-(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs - rhs._value : lhs - rhs._symbol;
    }

    /// @brief Substracts an expression from the value.
    inline friend GiNaC::ex operator-(const value_t &lhs, const GiNaC::ex &rhs)
    {
        return (lhs._replace) ? lhs._value - rhs : lhs._symbol - rhs;
    }

    /// @brief Returns the negation of the value.
    inline friend GiNaC::ex operator-(const value_t &lhs)
    {
        return -(lhs._replace) ? GiNaC::ex(lhs._value) : GiNaC::ex(lhs._symbol);
    }

    /// @brief Returns a relational.
    inline friend GiNaC::ex operator==(const GiNaC::ex &lhs, const value_t &rhs)
    {
        return (rhs._replace) ? lhs == rhs._value : lhs == rhs._symbol;
    }

    /// @brief Checks if two values are the same.
    inline friend bool operator==(const value_t &lhs, const value_t &rhs)
    {
        return lhs._symbol == rhs._symbol;
    }

    /// @brief Stream operator.
    inline friend std::ostream &operator<<(std::ostream &lhs, const value_t &rhs)
    {
        lhs << rhs._symbol;
        return lhs;
    }

private:
    /// @brief The unique GiNaC symbol.
    GiNaC::symbol _symbol;
    /// @brief The numerical value.
    double _value;
    /// @brief If we need to replace the symbol with the numerical value.
    bool _replace;
};

/// A list of values.
using value_list_t = std::vector<value_t>;

/// @brief Checks if the collection contains the given value.
/// @param collection the collection of values.
/// @param value the value we are looking for.
/// @return true if it is contained, false otherwise.
bool collection_contains_value(const value_list_t &collection, const value_t &value);

/// @brief Stream operator for a list of values.
std::ostream &operator<<(std::ostream &lhs, const value_list_t &rhs);

} // namespace symsolbin