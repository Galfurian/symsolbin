/// @file analog_model.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-01
///
/// @copyright Copyright (c) 2023
///

#include "symsolbin/solver/analog_model.hpp"
#include "symsolbin/solver/ginac_helper.hpp"
#include "symsolbin/solver/classifier.hpp"

namespace symsolbin
{

value_t ts("ts", .0, false);

static inline edge_list_t __collection_get_not_in(const edge_list_t &collection, const edge_list_t &other)
{
    edge_list_t::const_iterator cit0, cit1;
    edge_list_t list;
    bool not_in = true;
    for (cit0 = collection.begin(); cit0 != collection.end(); ++cit0) {
        not_in = true;
        for (cit1 = other.begin(); cit1 != other.end(); ++cit1) {
            if ((*cit0) == (*cit1)) {
                not_in = false;
                break;
            }
        }
        if (not_in)
            list.emplace_back(*cit0);
    }
    return list;
}

static inline edge_list_t __collection_get_connected_edges(const edge_list_t &collection, const node_t &node)
{
    edge_list_t::const_iterator cit;
    edge_list_t list;
    for (cit = collection.begin(); cit != collection.end(); ++cit)
        if (cit->is_connected_to(node))
            list.emplace_back(*cit);
    return list;
}

analog_model_t::analog_model_t()
    : system(),
      structure(),
      solution()
{
    // Nothing to do.
}

void analog_model_t::run_solver(const GiNaC::exmap &replacement)
{
    this->setup();
    this->solve(replacement);
}

inline void analog_model_t::__register_node(const node_t &node)
{
    if (!collection_contains_node(structure.nodes, node)) {
        structure.nodes.emplace_back(node);
    }
}

inline void analog_model_t::__register_value(const value_t &value)
{
    if (!collection_contains_value(system.values, value)) {
        system.values.emplace_back(value);
    }
}

inline void analog_model_t::__register_edge(const edge_t &edge)
{
    if (!collection_contains_edge(structure.edges, edge)) {
        structure.edges.emplace_back(edge);
        __register_node(edge.get_first());
        __register_node(edge.get_second());
    }
}

void analog_model_t::equations(const GiNaC::ex &e)
{
    static classifier_t classifier;
    if (classifier.classify(e)) {
        system.equations.emplace_back(GiNaC::ex_to<GiNaC::relational>(e));
    }
}

void analog_model_t::unknowns(const GiNaC::symbol &sym)
{
    system.unknowns.emplace_back(sym);
}

void analog_model_t::values(const value_t &value)
{
    __register_value(value);
}

GiNaC::symbol analog_model_t::P(const node_t &n1, const node_t &n2)
{
    __register_edge(edge_t(n1, n2, n1.get_name() + "_" + n2.get_name()));
    return ginac_helper::get_symbol(n1.get_name() + "_" + n2.get_name() + ".pot");
}

GiNaC::symbol analog_model_t::P(const edge_t &edge)
{
    __register_edge(edge);
    if (edge.get_alias().empty())
        return ginac_helper::get_symbol(edge.get_first().get_name() + "_" + edge.get_second().get_name() + ".pot");
    return ginac_helper::get_symbol(edge.get_alias() + ".pot");
}

GiNaC::symbol analog_model_t::F(const node_t &n1, const node_t &n2)
{
    __register_edge(edge_t(n1, n2, n1.get_name() + "_" + n2.get_name()));
    return ginac_helper::get_symbol(n1.get_name() + "_" + n2.get_name() + ".flw");
}

GiNaC::symbol analog_model_t::F(const edge_t &edge)
{
    __register_edge(edge);
    if (edge.get_alias().empty())
        return ginac_helper::get_symbol(edge.get_first().get_name() + "_" + edge.get_second().get_name() + ".flw");
    return ginac_helper::get_symbol(edge.get_alias() + ".flw");
}

GiNaC::ex analog_model_t::idt(const GiNaC::ex &e)
{
    auto idt    = value_t(name_gen::get_name("idt"));
    auto result = (idt + e * ts);
    solution.support.emplace_back(GiNaC::ex_to<GiNaC::relational>(idt == e * ts));
    if (!collection_contains_value(solution.values, idt)) {
        solution.values.emplace_back(idt);
    }
    return result;
}

GiNaC::ex analog_model_t::ddt(const GiNaC::ex &e)
{
    auto ddt    = value_t(name_gen::get_name("ddt"));
    auto result = ((e - ddt) / ts);
    solution.support.emplace_back(GiNaC::ex_to<GiNaC::relational>(ddt == (e - ddt) / ts));
    if (!collection_contains_value(solution.values, ddt)) {
        solution.values.emplace_back(ddt);
    }
    return result;
}

//inline GiNaC::ex limexp(const GiNaC::ex & e,
//                               std::string const & prefix = "")
//{
//    auto limexp_name = name_generator_t::get(prefix + "limexp");
//    auto limexp_var = ginac_helper::get_symbol(limexp_name);
//    auto result = (1 + e + limexp_var);
//    return result;
//}

equation_set_t analog_model_t::replace_symbols(const equation_set_t &equations, const GiNaC::exmap &replacement)
{
    equation_set_t replaced;
    for (auto &equation : equations) {
        replaced.emplace_back(
            GiNaC::ex_to<GiNaC::relational>(
                GiNaC::subs(
                    equation, replacement, GiNaC::subs_options::algebraic)));
    }
    return replaced;
}

std::ostream &operator<<(std::ostream &lhs, const analog_model_t &rhs)
{
    lhs << GiNaC::csrc_float;
    lhs << "analog_model_t:\n";
    lhs << "    Nodes  : ";
    for (const auto &it : rhs.structure.nodes)
        lhs << " " << it;
    lhs << "\n";
    lhs << "    Edges  : ";
    for (const auto &it : rhs.structure.edges)
        lhs << " " << it;
    lhs << "\n";
    lhs << "    Values : ";
    for (const auto &it : rhs.system.values)
        lhs << " " << it;
    lhs << "\n";
    lhs << "    Equations\n";
    for (const auto &it : rhs.system.equations)
        lhs << "        " << it << "\n";
    lhs << "    Equations (KPL)\n";
    for (const auto &it : rhs.system.kpl)
        lhs << "        " << it << "\n";
    lhs << "    Equations (KFL)\n";
    for (const auto &it : rhs.system.kfl)
        lhs << "        " << it << "\n";
    lhs << "    Unknowns\n";
    for (const auto &it : rhs.system.unknowns)
        lhs << "        " << it << "\n";
    if (!rhs.solution.equations.empty()) {
        lhs << "    Results\n";
        for (const auto &it : rhs.solution.equations)
            lhs << "        " << it << "\n";
    }
    return lhs;
}

void analog_model_t::solve(const GiNaC::exmap &replacement)
{
    this->compute_kfl();
    this->compute_kpl();

    // Gather the equations.
    GiNaC::lst equations;
    for (const auto &it : system.equations)
        equations.append(it);
    for (const auto &it : system.kpl)
        equations.append(it);
    for (const auto &it : system.kfl)
        equations.append(it);

    // Gather the unknowns.
    GiNaC::lst unknowns;
    for (const auto &it : system.unknowns)
        unknowns.append(it);

    // Run the solver.
    solution.equations =
        ginac_helper::split_solved(
            ginac_helper::solve(
                equations,
                unknowns,
                GiNaC::solve_algo::automatic,
                replacement));
}

void analog_model_t::compute_kfl()
{
    system.kfl.clear();

    for (const auto &node : structure.nodes) {
        if (node.is_ground())
            continue;
        GiNaC::ex sum;
        for (const auto &edge : structure.edges) {
            if (edge.get_first() == node) {
                sum = sum - F(edge);
            } else if (edge.get_second() == node) {
                sum = sum + F(edge);
            }
        }
        system.kfl.emplace_back(sum == 0);
    }
}

struct parallel_info_t {
    /// The representative edge of a group.
    edge_t representative;
    /// The other edges of the group.
    edge_list_t others;

    parallel_info_t(edge_t _representative, edge_list_t _others)
        : representative(std::move(_representative)),
          others(std::move(_others))
    {
        // Nothing to do.
    }
};

struct selector_t {
    node_list_t selected_node;
    edge_list_t selected_edge;

    bool select(const node_t &node)
    {
        if (!this->is_selected(node)) {
            selected_node.emplace_back(node);
            return true;
        }
        return false;
    }

    bool remove(const node_t &node)
    {
        if (!this->is_selected(node))
            return true;
        for (auto it = selected_node.begin(); it != selected_node.end(); ++it) {
            if (it->get_name() == node.get_name()) {
                selected_node.erase(it);
                return true;
            }
        }
        return false;
    }

    bool select(const edge_t &edge)
    {
        if (!this->is_selected(edge)) {
            selected_edge.emplace_back(edge);
            return true;
        }
        return false;
    }

    bool remove(const edge_t &edge)
    {
        if (!is_selected(edge))
            return true;
        for (auto it = selected_edge.begin(); it != selected_edge.end(); ++it) {
            if (it->get_alias() == edge.get_alias()) {
                selected_edge.erase(it);
                return true;
            }
        }
        return false;
    }

    bool is_selected(const node_t &node) const
    {
        for (const auto &it : selected_node)
            if (it.get_name() == node.get_name())
                return true;
        return false;
    }

    bool is_selected(const edge_t &edge) const
    {
        for (const auto &it : selected_edge)
            if (it.get_alias() == edge.get_alias())
                return true;
        return false;
    }

    bool is_parallel(const edge_t &edge) const
    {
        for (const auto &it : selected_edge)
            if (it.is_parallel_to(edge))
                return true;
        return false;
    }

    /// @brief Stream operator.
    inline friend std::ostream &operator<<(std::ostream &lhs, const selector_t &rhs)
    {
        lhs << "[NODES : {";
        for (const auto &it : rhs.selected_node)
            lhs << " " << it;
        lhs << "}, EDGES : {";
        for (const auto &it : rhs.selected_edge)
            lhs << " " << it;
        lhs << "}]";
        return lhs;
    }
};

void __find_minimum_spanning_tree_ground_analysis(
    const edge_list_t &edges,
    const node_list_t &,
    selector_t &selected)
{
    // Prepare a function which checks if a edge needs to be skipped.
    auto __need_to_skip = [&selected](const edge_t &edge) {
        // Check if none of the edge nodes is a ground node.
        if ((!edge.get_first().is_ground()) && (!edge.get_second().is_ground())) {
            return true;
        }
        // Check if both the edge nodes are selected.
        if (selected.is_selected(edge.get_first()) && selected.is_selected(edge.get_second())) {
            return true;
        }
        // Check if the given edge is connected to an already selected node
        // AND the other node of the edge is a ground node.
        for (const edge_t &sel_edge : selected.selected_edge) {
            if (sel_edge.is_connected_to(edge.get_first()) && edge.get_second().is_ground()) {
                return true;
            }
            if (sel_edge.is_connected_to(edge.get_second()) && edge.get_first().is_ground()) {
                return true;
            }
        }
        return false;
    };
    for (const edge_t &edge : edges) {
        // Check if the edge needs to be skipped.
        if (!__need_to_skip(edge)) {
            // Set that the nodes of the edge and the edge have been selected.
            selected.select(edge);
            selected.select(edge.get_first());
            selected.select(edge.get_second());
        }
    }
}

selector_t __find_minimum_spanning_tree(
    const edge_list_t &edges,
    const node_list_t &nodes)
{
    selector_t selected;

    // Find the edges that are connected to ground.
    __find_minimum_spanning_tree_ground_analysis(edges, nodes, selected);

    // The first iteration is through all the nodes which have been selected.
    for (const auto &node : selected.selected_node) {
        // Get the edges that did not get selected.
        for (const auto &edge : __collection_get_connected_edges(__collection_get_not_in(edges, selected.selected_edge), node)) {
            // Then analyze the sectond node.
            if ((node == edge.get_first()) && !selected.is_selected(edge.get_second())) {
                // Push the edge inside the list of take edges.
                selected.select(edge);
                // Set the node as selected.
                selected.select(edge.get_second());
            }
            // Then analyze the first node.
            else if ((node == edge.get_second()) && !selected.is_selected(edge.get_first())) {
                // Push the edge inside the list of take edges.
                selected.select(edge);
                // Set the node as selected.
                selected.select(edge.get_first());
            }
        }
    }
    if (selected.selected_edge.size() != (nodes.size() - 1))
        std::cerr << "Number of selected edges is lower than expected (" << selected.selected_edge.size() << " vs " << (nodes.size() - 1) << ")...\n";
    return selected;
}

static bool __find_graph_loop(
    const edge_list_t &edges,
    const node_list_t &nodes,
    selector_t &seleted,
    selector_t &visited,
    edge_list_t &positive,
    edge_list_t &negative,
    const node_t &node)
{
    // We have encountered an already visited node.
    if (visited.is_selected(node)) {
        return true;
    }
    // Add the node to the list of visited nodes.
    visited.select(node);

    // Iterate through the list of connected edges.
    for (const auto &edge : __collection_get_connected_edges(edges, node)) {
        // If the current edge has not been selected, skip it.
        if (!seleted.is_selected(edge))
            continue;

        // If the current edge has already been visited, skip it.
        if (visited.is_selected(edge))
            continue;

        // If the current edge is in paralle to an already visited edge, skip it.
        if (visited.is_parallel(edge))
            continue;

        // Set the current edge as a visited edge.
        visited.select(edge);

        // Recursively call the algorithm on the next node (node 2).
        if (node == edge.get_first()) {
            if (__find_graph_loop(edges, nodes, seleted, visited, positive, negative, edge.get_second())) {
                // Add the edge to the list of positive edges.
                positive.emplace_back(edge);
                return true;
            }
        }
        // Recursively call the algorithm on the next node (node 1).
        if (node == edge.get_second()) {
            if (__find_graph_loop(edges, nodes, seleted, visited, positive, negative, edge.get_first())) {
                // Add the edge to the list of negative edges.
                negative.emplace_back(edge);
                return true;
            }
        }

        // Set the current edge as a not visited edge.
        visited.remove(edge);
    }

    // If we've not found a cycle, just remove the node from the list
    //  of visited nodes, so it can be picked again.
    visited.remove(node);

    // We have not found a valid path.
    return false;
}

std::vector<std::pair<edge_list_t, edge_list_t>> __find_graph_loops(
    const edge_list_t &edges,
    const node_list_t &nodes,
    const selector_t &selected)
{
    //std::cout << "Searching graph cycles...\n";
    // Prepare a vector of loops.
    std::vector<std::pair<edge_list_t, edge_list_t>> loops;

    // Add one by one the unselected edges.
    for (auto edge : __collection_get_not_in(edges, selected.selected_edge)) {
        // Create a copy of the selected.
        selector_t tmp = selected, visited;
        // Set the edge as selected.
        tmp.select(edge);
        // Create an empty loop.
        edge_list_t positive, negative;
        // Try to find a loop starting from the second node of the edge.
        __find_graph_loop(edges, nodes, tmp, visited, positive, negative, edge.get_second());
        // Add the loop only if the loop is not empty.
        if (positive.empty() && negative.empty()) {
            std::cerr << "Cannot find a graph cycle.\n";
        } else {
            // Add the loop to the vector of loops.
            loops.emplace_back(std::make_pair(positive, negative));
        }
    }
    return loops;
}

std::vector<parallel_info_t> __find_parallel_edges(const edge_list_t &edges)
{
    std::vector<parallel_info_t> parallel;
    std::set<edge_t> analyzed;

    for (const edge_t &edge : edges) {
        if (analyzed.count(edge))
            continue;

        analyzed.insert(edge);

        edge_list_t others;

        for (const edge_t &other : edges) {
            if (analyzed.count(other))
                continue;
            if (!edge.is_parallel_to(other))
                continue;
            analyzed.insert(other);
            others.emplace_back(other);
        }
        if (!others.empty()) {
            parallel.emplace_back(parallel_info_t(edge, others));
        }
    }
    return parallel;
}

void analog_model_t::compute_kpl()
{
    system.kpl.clear();
    auto selected = __find_minimum_spanning_tree(structure.edges, structure.nodes);
    auto loops    = __find_graph_loops(structure.edges, structure.nodes, selected);
    for (const auto &loop_pair : loops) {
        GiNaC::ex sum;
        for (const auto &edge : loop_pair.first)
            sum = sum + P(edge);
        for (const auto &edge : loop_pair.second)
            sum = sum - P(edge);
        system.kpl.emplace_back(sum == 0);
    }
    auto parallel_list = __find_parallel_edges(structure.edges);
    for (const auto &group : parallel_list) {
        for (const edge_t &edge : group.others) {
            system.kpl.emplace_back(P(group.representative) == P(edge));
        }
    }
}

} // namespace symsolbin
