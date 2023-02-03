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

/// @brief Details about the structure of a system.
struct structure_t {
    /// The edges of the system.
    edge_list_t edges;
    /// The nodes of the system.
    node_list_t nodes;
};

/// @brief Details about a system of equations.
struct system_t {
    /// The initial set of equations.
    equation_set_t equations;
    /// Kirchoff's potential law equations.
    equation_set_t kpl;
    /// Kirchoff's flow law equations.
    equation_set_t kfl;
    /// The unknowns of the system.
    symbol_set_t unknowns;
    /// The list of values.
    value_list_t values;
};

/// @brief Details about a solved system of equations.
struct solved_systyem_t {
    /// The solved set of equations.
    equation_set_t equations;
    /// Support equations for the solved set.
    equation_set_t support;
    /// The list of support values.
    value_list_t values;
};

/// @brief An analog model.
class analog_model_t {
public:
    /// @brief Constructor.
    analog_model_t();

    /// @brief runs the solver.
    /// @return the solved set of equations.
    void run_solver(const GiNaC::exmap &replacement = GiNaC::exmap());

    /// @brief Streams operator for an analog model.
    friend std::ostream &operator<<(std::ostream &lhs, const analog_model_t &rhs);

    /// @brief Returns the system of equations.
    inline system_t get_system() const
    {
        return system;
    }

    /// @brief Returns the structure of the circuit.
    inline structure_t get_structure() const
    {
        return structure;
    }

    /// @brief Returns the solution to the system of equations.
    inline solved_systyem_t get_solution() const
    {
        return solution;
    }

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

    /// @brief Creates an integral.
    /// @param e the expression inside the integral.
    /// @return the discretized expression.
    GiNaC::ex idt(const GiNaC::ex &e);

    /// @brief Creates a derivative.
    /// @param e the expression inside the derivative.
    /// @return the discretized expression.
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

    /// @brief Registers a value in the system.
    /// @param value the value we want to register.
    void values(const value_t &value);

    /// @brief Registers a value in the system.
    /// @param value the value we want to register.
    /// @param args the other values.
    template <typename... Args>
    void values(const value_t &value, Args... args)
    {
        values(value);
        values(args...);
    }

    /// @brief Replaces the symbols inside the equations.
    /// @param equations the equations to edit.
    /// @param replacement the replacement for symbols.
    /// @return the equations with all the specified symbols replaced.
    equation_set_t replace_symbols(const equation_set_t &equations, const GiNaC::exmap &replacement);

private:
    /// @brief System of equations.
    system_t system;
    /// @brief Structure of the circuit.
    structure_t structure;
    /// @brief Solution to the system of equations.
    solved_systyem_t solution;

    void __register_node(const node_t &node);

    void __register_edge(const edge_t &edge);

    void __register_value(const value_t &value);

    /// @brief
    void compute_kfl();

    /// @brief
    void compute_kpl();

    /// @brief Solves the system of equations.
    void solve(const GiNaC::exmap &replacement = GiNaC::exmap());
};

} // namespace symsolbin