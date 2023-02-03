/// @file analog_pair.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Variable which holds potential and flow.
/// @copyright (c) 2014-2023 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#pragma once

#include "double_op.hpp"

#include <ostream>

namespace symsolbin
{

using analog_value_t = double;

/// @brief Keeps track of potential and flow an analog edge.
struct analog_pair_t {
    /// @brief Potential value.
    analog_value_t pot;
    /// @brief Flow value.
    analog_value_t flw;

    /// @brief Construct a new pair of values.
    analog_pair_t()
        : pot(),
          flw()
    {
        // Nothing to do.
    }

    /// @brief Construct and initializes a new pair of values.
    /// @param _pot initial potential value.
    /// @param _flw initial flow value.
    analog_pair_t(analog_value_t _pot, analog_value_t _flw)
        : pot(_pot),
          flw(_flw)
    {
        // Nothing to do.
    }

    ~analog_pair_t() = default;

    /// @brief Computes power for the given analog edge.
    inline analog_value_t power() const
    {
        return (pot * flw);
    }

    /// @brief Checks equality between two analog edges.
    /// @param other the other analog edge.
    /// @return if the values between the two analog edges are equal.
    inline bool operator==(const analog_pair_t &other) const
    {
        return is_equal(pot, other.pot) && is_equal(flw, other.flw);
    }

    /// @brief Checks inequality between two analog edges.
    /// @param other the other analog edge.
    /// @return if the values between the two analog edges are different.
    inline bool operator!=(const analog_pair_t &other) const
    {
        return (!is_equal(pot, other.pot)) || (!is_equal(flw, other.flw));
    }

    /// @brief Sums the potential and flow values of two analog edges.
    /// @param other the other analog edge.
    /// @return this edge.
    inline analog_pair_t &operator+=(const analog_pair_t &other)
    {
        pot += other.pot;
        flw += other.flw;
        return (*this);
    }

    /// @brief Substracts the potential and flow values of two analog edges.
    /// @param other the other analog edge.
    /// @return this edge.
    inline analog_pair_t &operator-=(const analog_pair_t &other)
    {
        pot -= other.pot;
        flw -= other.flw;
        return (*this);
    }

    /// @brief Multiplies the potential and flow values of two analog edges.
    /// @param other the other analog edge.
    /// @return this edge.
    inline analog_pair_t &operator*=(const analog_pair_t &other)
    {
        pot *= other.pot;
        flw *= other.flw;
        return (*this);
    }

    /// @brief Stream operator.
    inline friend std::ostream &operator<<(std::ostream &out, const analog_pair_t &other)
    {
        out << other.pot << ";" << other.flw;
        return out;
    }
};

} // namespace symsolbin