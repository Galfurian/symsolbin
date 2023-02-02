/// @file   double_op.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief  Functions used to deal with equality between double values.
/// @copyright (c) 2014-2023 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#pragma once

#include <algorithm>
#include <cmath>

/// @brief Checks if two floating-point values are equal.
/// @param a the first value.
/// @param b the secnd value.
/// @param tolerance the tolerance for the check.
/// @return true if they are considered equal, false otherwise.
inline bool is_equal(double a, double b, double tolerance = 1e-09)
{
#if 1
    return (static_cast<int>(std::max(std::abs(a), std::abs(b)) / tolerance))
               ? (std::abs(a - b) < tolerance)
               : true;
#else
    return std::abs(a - b) < tolerance;
#endif
}

/// @brief Checks if a floating-point value is lesser or equal to another.
/// @param a the first value.
/// @param b the secnd value.
/// @param tolerance the tolerance for the check.
/// @return true if the first value is lesser or equal to the second value, false otherwise.
inline bool is_lequal(double a, double b, double tolerance = 1e-09)
{
    return is_equal(a, b, tolerance) || (a < b);
}

/// @brief Checks if a floating-point value is greater or equal to another.
/// @param a the first value.
/// @param b the secnd value.
/// @param tolerance the tolerance for the check.
/// @return true if the first value is greater or equal to the second value, false otherwise.
inline bool is_gequal(double a, double b, double tolerance = 1e-09)
{
    return is_equal(a, b, tolerance) || (a > b);
}
