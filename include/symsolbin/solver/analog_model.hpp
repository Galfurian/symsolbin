/// @file analog_model.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @copyright (c) 2014-2023 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#pragma once

#include "symsolbin/structure/value.hpp"
#include "symsolbin/structure/edge.hpp"

namespace symsolbin
{

/// A set of equations.
using equation_set_t = std::vector<GiNaC::relational>;
/// A set of symbols.
using symbol_set_t = std::vector<GiNaC::symbol>;

/// Details of a model.
struct model_details_t {
    /// The equations of the system.
    GiNaC::lst equations;
    /// The unknowns of the system.
    GiNaC::lst unknowns;
    /// The edges of the system.
    edge_list_t edges;
    /// The nodes of the system.
    node_list_t nodes;
    /// The list of values.
    value_list_t values;
};

/// @brief An analog model.
class analog_model_t {
public:
    analog_model_t()
        : __equations(),
          __unknowns(),
          __kpl(),
          __kfl(),
          __nodes(),
          __edges(),
          __values(),
          __result(),
          __result_support()
    {
    }

    /// @brief runs the solver.
    /// @return the solved set of equations.
    inline const equation_set_t &run_solver(const GiNaC::exmap &replacement = GiNaC::exmap())
    {
        this->setup();
        this->solve(replacement);
        return __result;
    }

    /// @brief returns the details of the model.
    /// @return the details of the model.
    model_details_t get_model_details(const GiNaC::exmap &replacement = GiNaC::exmap());

    /// @brief Get the result of the solved set.
    /// @return the solved set of equations.
    inline const equation_set_t &get_result()
    {
        return __result;
    }

    /// @brief Prints the results.
    void print_result() const;

    friend std::ostream &operator<<(std::ostream &lhs, const analog_model_t &rhs);

protected:
    /// @brief Setup the equation system.
    virtual void setup() = 0;

    /// @brief Creates a potential access variable between two nodes.
    /// @param n1 first node.
    /// @param n2 second node.
    /// @return The GiNaC symbol for the accessed value.
    GiNaC::symbol P(const node_t &n1, const node_t &n2);

    /// @brief Creates a potential access variable for a edge.
    /// @param edge the edge.
    /// @return The GiNaC symbol for the accessed value.
    GiNaC::symbol P(const edge_t &edge);

    /// @brief Creates a flow access variable between two nodes.
    /// @param n1 first node.
    /// @param n2 second node.
    /// @return The GiNaC symbol for the accessed value.
    GiNaC::symbol F(const node_t &n1, const node_t &n2);

    /// @brief Creates a flow access variable for a edge.
    /// @param edge the edge.
    /// @return The GiNaC symbol for the accessed value.
    GiNaC::symbol F(const edge_t &edge);
    
    GiNaC::ex idt(const GiNaC::ex &e);
    
    GiNaC::ex ddt(const GiNaC::ex &e);

    /// @brief Defines an equation of the system of equations.
    /// @param e the equation.
    void equations(const GiNaC::ex &e);

    /// @brief Defines a set of equations of the system of equations.
    /// @param e the equation.
    /// @param args the other equations.
    template <typename... Args>
    void equations(const GiNaC::ex &e, Args... args)
    {
        equations(e);
        equations(args...);
    }

    /// @brief Defines an unknown of the system of equations.
    /// @param sym the symbol.
    void unknowns(const GiNaC::symbol &sym);

    /// @brief Defines an unknown of the system of equations.
    /// @param sym the symbol.
    /// @param args the other unknowns.
    template <typename... Args>
    void unknowns(const GiNaC::symbol &sym, Args... args)
    {
        unknowns(sym);
        unknowns(args...);
    }

    /// @brief Replaces the symbols inside the equations.
    /// @param equations the equations to edit.
    /// @param replacement the replacement for symbols.
    /// @return the equations with all the specified symbols replaced.
    equation_set_t replace_symbols(const equation_set_t &equations, const GiNaC::exmap &replacement);

private:
    ///
    GiNaC::lst __equations;
    ///
    GiNaC::lst __unknowns;
    ///
    GiNaC::lst __kpl;
    ///
    GiNaC::lst __kfl;
    ///
    node_list_t __nodes;
    ///
    edge_list_t __edges;
    ///
    value_list_t __values;
    /// The solved set of equations.
    equation_set_t __result;
    /// Support functions for the solved set.
    equation_set_t __result_support;

    void __register_node(const node_t &node);

    void __register_edge(const edge_t &edge);

    void __register_real(const value_t &value);

    /// @brief
    void compute_kfl();

    /// @brief
    void compute_kpl();

    /// @brief Solves the system of equations.
    void solve(const GiNaC::exmap &replacement = GiNaC::exmap());
};

} // namespace symsolbin