/// @file   ginac_helper.hpp
/// @brief  Functions used to help interfacing with GiNaC.
/// @author Enrico Fraccaroli
/// @copyright
/// Copyright (c) 2017-2021 Enrico Fraccaroli <enry.frak@gmail.com>
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///     The above copyright notice and this permission notice shall be included
///     in all copies or substantial portions of the Software.
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.

#pragma once

#include "symsolbin/solver/name_generator.hpp"
#include "symsolbin/structure/value.hpp"

#include <ginac/ginac.h>
#include <fstream>
#include <vector>

namespace symsolbin
{

/// @brief The time step variable.
extern value_t ts;

/// @brief A set of equations.
using equation_set_t = std::vector<GiNaC::relational>;

/// @brief Stream operator for ginac relational.
inline std::ostream &operator<<(std::ostream &lhs, const GiNaC::relational &rhs)
{
    lhs << rhs.lhs() << " = " << rhs.rhs();
    return lhs;
}

namespace ginac_helper
{

/// @brief Creates a new GiNaC symbol for the given name.
/// @param name the name of the new symbol.
/// @return the newly created symbol.
inline GiNaC::symbol &get_symbol(std::string const &name)
{
    static std::map<std::string, GiNaC::symbol> symbols;
    auto it = symbols.find(name);
    if (it == symbols.end()) {
        return symbols.insert(std::make_pair(name, GiNaC::symbol(name))).first->second;
    }
    return it->second;
}

/// @brief Solves the system of equations.
/// @param equations the list of equations.
/// @param symbols the unknowns.
/// @param options the algorithm.
/// @param replacement symbol replacement.
/// @return the solved set.
inline GiNaC::ex solve(const GiNaC::ex &equations,
                       const GiNaC::ex &symbols,
                       unsigned options                = GiNaC::solve_algo::automatic,
                       const GiNaC::exmap &replacement = GiNaC::exmap())
{
    if (replacement.empty())
        return GiNaC::lsolve(equations, symbols, options);
    return GiNaC::lsolve(GiNaC::subs(equations, replacement, GiNaC::subs_options::algebraic), symbols, options);
}

/// @brief Splits the set of solved equations into a vector.
/// @param equations the set to split.
/// @return the vector contaning the equations.
inline equation_set_t split_solved(const GiNaC::ex &equations)
{
    equation_set_t set;
    for (auto it = equations.begin(); it != equations.end(); ++it)
        set.emplace_back(GiNaC::ex_to<GiNaC::relational>(*it));
    return set;
}

} // namespace ginac_helper

} // namespace symsolbin