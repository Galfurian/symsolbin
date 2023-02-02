/// @file classifier.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief 
/// @copyright (c) 2014-2023 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#pragma once

#include "ginac/ginac.h"

namespace symsolbin
{

/// @brief Visitor which allows to classify an equation.
class classifier_t : public GiNaC::visitor {
public:
    /// @brief Constructor.
    explicit classifier_t();

    /// @brief Destructor.
    ~classifier_t() override;

    bool classify(const GiNaC::ex &e)
    {
        return this->visit(e);
    }

private:
    /// @brief Visiting function for element of type ex.
    bool visit(const GiNaC::ex &e);

    /// @brief Visiting function for element of type relational.
    bool visitRelational(const GiNaC::relational &e);

    /// @brief Visiting function for element of type symbol.
    bool visitSymbol(const GiNaC::symbol &e);

    /// @brief Visiting function for element of type constant.
    bool visitConstant(const GiNaC::constant &e);

    /// @brief Visiting function for a series of summations.
    bool visitAdd(const GiNaC::ex &e);

    /// @brief Visiting function for a series of multiplication.
    bool visitMul(const GiNaC::ex &e);

    /// @brief Visiting function for element of type numeric.
    bool visitNumeric(const GiNaC::numeric &e);

    /// @brief Visiting function for element of type function.
    bool visitFunction(const GiNaC::function &e);

    /// @brief Visiting function for element of type power.
    bool visitPower(const GiNaC::power &e);
};

} // namespace symsolbin