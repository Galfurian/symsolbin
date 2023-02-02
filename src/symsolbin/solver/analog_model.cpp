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
        if ((cit->n1 == node) || (cit->n2 == node))
            list.emplace_back(*cit);
    return list;
}

inline void analog_model_t::__register_node(const node_t &node)
{
    if (!collection_contains_node(__nodes, node)) {
        __nodes.emplace_back(node);
    }
}

inline void analog_model_t::__register_real(const value_t &real)
{
    if (!collection_contains_value(__values, real)) {
        __values.emplace_back(real);
    }
}

inline void analog_model_t::__register_edge(const edge_t &edge)
{
    if (!collection_contains_edge(__edges, edge)) {
        __edges.emplace_back(edge);
        __register_node(edge.n1);
        __register_node(edge.n2);
    }
}

void analog_model_t::equations(const GiNaC::ex &e)
{
    static classifier_t classifier;
    if (classifier.classify(e))
        __equations.append(e);
}

void analog_model_t::unknowns(const GiNaC::symbol &sym)
{
    __unknowns.append(sym);
}

GiNaC::symbol analog_model_t::P(const node_t &n1, const node_t &n2)
{
    __register_edge(edge_t(n1, n2, n1.name + "_" + n2.name));
    return ginac_helper::get_symbol(n1.name + "_" + n2.name + ".pot");
}

GiNaC::symbol analog_model_t::P(const edge_t &edge)
{
    __register_edge(edge);
    if (edge.alias.empty())
        return ginac_helper::get_symbol(edge.n1.name + "_" + edge.n2.name + ".pot");
    return ginac_helper::get_symbol(edge.alias + ".pot");
}

GiNaC::symbol analog_model_t::F(const node_t &n1, const node_t &n2)
{
    __register_edge(edge_t(n1, n2, n1.name + "_" + n2.name));
    return ginac_helper::get_symbol(n1.name + "_" + n2.name + ".flw");
}

GiNaC::symbol analog_model_t::F(const edge_t &edge)
{
    __register_edge(edge);
    if (edge.alias.empty())
        return ginac_helper::get_symbol(edge.n1.name + "_" + edge.n2.name + ".flw");
    return ginac_helper::get_symbol(edge.alias + ".flw");
}

GiNaC::ex analog_model_t::idt(const GiNaC::ex &e)
{
    auto idt_name = name_generator_t::get("idt");
    auto idt      = ginac_helper::get_symbol(idt_name);
    auto result   = (idt + e * ts);
    __result_support.emplace_back(idt == e * ts);
    return result;
}

GiNaC::ex analog_model_t::ddt(const GiNaC::ex &e)
{
    auto ddt_name = name_generator_t::get("ddt");
    auto ddt      = ginac_helper::get_symbol(ddt_name);
    auto result   = ((e - ddt) / ts);
    __result_support.emplace_back(ddt == (e - ddt) / ts);
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

void analog_model_t::print_result() const
{
    std::cout << "Solved equations:\n";
    std::cout << GiNaC::csrc_float;
    for (const auto &equation : __result) {
        std::cout << "    " << equation << "\n";
    }
    std::cout << "Support:\n";
    for (const auto &equation : __result_support) {
        std::cout << "    " << equation << "\n";
    }
}

std::ostream &operator<<(std::ostream &lhs, const analog_model_t &rhs)
{
    lhs << GiNaC::csrc_float;
    lhs << "analog_model_t:\n";
    lhs << "    Nodes  : ";
    for (const auto &it : rhs.__nodes)
        lhs << " " << it;
    lhs << "\n";
    lhs << "    Edges  : ";
    for (const auto &it : rhs.__edges)
        lhs << " " << it;
    lhs << "\n";
    lhs << "    Values : ";
    for (const auto &it : rhs.__values)
        lhs << " " << it;
    lhs << "\n";
    lhs << "    Equations\n";
    for (const auto &it : rhs.__equations)
        lhs << "        " << it << "\n";
    lhs << "    Equations (KPL)\n";
    for (const auto &it : rhs.__kpl)
        lhs << "        " << it << "\n";
    lhs << "    Equations (KFL)\n";
    for (const auto &it : rhs.__kfl)
        lhs << "        " << it << "\n";
    lhs << "    Unknowns\n";
    for (const auto &it : rhs.__unknowns)
        lhs << "        " << it << "\n";
    if (!rhs.__result.empty()) {
        lhs << "    Results\n";
        for (const auto &it : rhs.__result)
            lhs << "        " << it << "\n";
    }
    return lhs;
}

model_details_t analog_model_t::get_model_details(const GiNaC::exmap &)
{
    // Build the system.
    this->setup();
    this->compute_kfl();
    this->compute_kpl();
    // Fill the details.
    model_details_t details;
    for (const auto &it : __equations)
        details.equations.append(GiNaC::ex_to<GiNaC::relational>(it));
    for (const auto &it : __kpl)
        details.equations.append(GiNaC::ex_to<GiNaC::relational>(it));
    for (const auto &it : __kfl)
        details.equations.append(GiNaC::ex_to<GiNaC::relational>(it));
    for (const auto &it : __unknowns)
        details.unknowns.append(GiNaC::ex_to<GiNaC::symbol>(it));
    details.nodes = this->__nodes;
    details.edges = this->__edges;
    return details;
}

void analog_model_t::solve(const GiNaC::exmap &replacement)
{
    this->compute_kfl();
    this->compute_kpl();

    GiNaC::lst equation_list;
    for (const auto &it : __equations)
        equation_list.append(it);
    for (const auto &it : __kpl)
        equation_list.append(it);
    for (const auto &it : __kfl)
        equation_list.append(it);

    std::cout << *this << "\n";

    __result =
        ginac_helper::split_solved(
            ginac_helper::solve(
                equation_list,
                __unknowns,
                GiNaC::solve_algo::automatic,
                replacement));
}

void analog_model_t::compute_kfl()
{
    __kfl.remove_all();

    for (const auto &node : __nodes) {
        if (node.ground)
            continue;
        GiNaC::ex sum;
        for (const auto &edge : __edges) {
            if (edge.n1 == node) {
                sum = sum - F(edge);
            } else if (edge.n2 == node) {
                sum = sum + F(edge);
            }
        }
        __kfl.append(sum == 0);
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
        if (!is_selected(node)) {
            selected_node.emplace_back(node);
            return true;
        }
        return false;
    }

    bool remove(const node_t &node)
    {
        if (!is_selected(node))
            return true;
        for (auto it = selected_node.begin(); it != selected_node.end(); ++it) {
            if (it->name == node.name) {
                selected_node.erase(it);
                return true;
            }
        }
        return false;
    }

    bool select(const edge_t &edge)
    {
        if (!is_selected(edge)) {
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
            if (it->alias == edge.alias) {
                selected_edge.erase(it);
                return true;
            }
        }
        return false;
    }

    bool is_selected(const node_t &node) const
    {
        for (const auto &it : selected_node)
            if (it.name == node.name)
                return true;
        return false;
    }

    bool is_selected(const edge_t &edge) const
    {
        for (const auto &it : selected_edge)
            if (it.alias == edge.alias)
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
};

void __find_minimum_spanning_tree_ground_analysis(
    const edge_list_t &__edges,
    const node_list_t &,
    selector_t &selected)
{
    // Prepare a function which checks if a edge needs to be skipped.
    auto __need_to_skip = [&selected](const edge_t &edge) {
        // Check if none of the edge nodes is a ground node.
        if ((!edge.n1.ground) && (!edge.n2.ground)) {
            //std::cout << "[none is ground]";
            return true;
        }
        // Check if both the edge nodes are selected.
        if (selected.is_selected(edge.n1) && selected.is_selected(edge.n2)) {
            //std::cout << "[both already selected]";
            return true;
        }
        // Check if the given edge is connected to an already selected node
        // AND the other node of the edge is a ground node.
        for (const edge_t &sel_edge : selected.selected_edge) {
            if (sel_edge.is_connected_to(edge.n1) && edge.n2.ground)
                return true;
            if (sel_edge.is_connected_to(edge.n2) && edge.n1.ground)
                return true;
        }
        return false;
    };
    for (const edge_t &edge : __edges) {
        // Check if the edge needs to be skipped.
        if (!__need_to_skip(edge)) {
            // Set that the nodes of the edge and the edge have been selected.
            selected.select(edge);
            selected.select(edge.n1);
            selected.select(edge.n2);
        }
    }
}

selector_t __find_minimum_spanning_tree(
    const edge_list_t &__edges,
    const node_list_t &__nodes)
{
    selector_t selected;

    // Find the edges that are connected to ground.
    __find_minimum_spanning_tree_ground_analysis(__edges, __nodes, selected);

    // The first iteration is through all the nodes which have been selected.
    for (const auto &node : selected.selected_node) {
        // Get the edges that did not get selected.
        for (const auto &edge : __collection_get_connected_edges(__collection_get_not_in(__edges, selected.selected_edge), node)) {
            // Then analyze the sectond node.
            if ((node == edge.n1) && !selected.is_selected(edge.n2)) {
                // Push the edge inside the list of take edges.
                selected.select(edge);
                // Set the node as selected.
                selected.select(edge.n2);
            }
            // Then analyze the first node.
            else if ((node == edge.n2) && !selected.is_selected(edge.n1)) {
                // Push the edge inside the list of take edges.
                selected.select(edge);
                // Set the node as selected.
                selected.select(edge.n1);
            }
        }
    }
    if (selected.selected_edge.size() != (__nodes.size() - 1))
        std::cout << "Number of selected edges is lower than expected (" << selected.selected_edge.size() << " vs " << (__nodes.size() - 1) << ")...\n";
    return selected;
}

static bool __find_graph_loop(
    const edge_list_t &__edges,
    const node_list_t &__nodes,
    selector_t &seleted,
    selector_t &visited,
    edge_list_t &positive,
    edge_list_t &negative,
    const node_t &node)
{
    // We have encountered an already visited node.
    if (visited.is_selected(node)) {
        //std::cout << "A cycle has been found!\n";
        return true;
    }
    // Add the node to the list of visited nodes.
    visited.select(node);

    // Iterate through the list of connected edges.
    for (const auto &edge : __collection_get_connected_edges(__edges, node)) {
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
        if (node == edge.n1) {
            if (__find_graph_loop(__edges, __nodes, seleted, visited, positive, negative, edge.n2)) {
                // Add the edge to the list of positive edges.
                positive.emplace_back(edge);
                return true;
            }
        }
        // Recursively call the algorithm on the next node (node 1).
        if (node == edge.n2) {
            if (__find_graph_loop(__edges, __nodes, seleted, visited, positive, negative, edge.n1)) {
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
    const edge_list_t &__edges,
    const node_list_t &__nodes,
    const selector_t &selected)
{
    //std::cout << "Searching graph cycles...\n";
    // Prepare a vector of loops.
    std::vector<std::pair<edge_list_t, edge_list_t>> loops;

    // Add one by one the unselected edges.
    for (auto edge : __collection_get_not_in(__edges, selected.selected_edge)) {
        //std::cout << "Adding `" << edge << "` and searching for a loop...\n";
        // Create a copy of the selected.
        selector_t tmp = selected, visited;
        // Set the edge as selected.
        tmp.select(edge);
        // Create an empty loop.
        edge_list_t positive, negative;
        // Try to find a loop starting from the second node of the edge.
        __find_graph_loop(__edges, __nodes, tmp, visited, positive, negative, edge.n2);
        // Add the loop only if the loop is not empty.
        if (positive.empty() && negative.empty()) {
            //std::cout << "Cannot find a graph cycle.\n";
        } else {
            // Add the loop to the vector of loops.
            loops.emplace_back(std::make_pair(positive, negative));
            //std::cout << "Loop : " << positive << " - " << negative << "\n";
        }
    }
    return loops;
}

std::vector<parallel_info_t> __find_parallel_edges(const edge_list_t &__edges)
{
    std::vector<parallel_info_t> parallel;
    std::set<edge_t> analyzed;

    for (const edge_t &edge : __edges) {
        if (analyzed.count(edge))
            continue;

        analyzed.insert(edge);

        edge_list_t others;

        for (const edge_t &other : __edges) {
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
    __kpl.remove_all();
    auto selected = __find_minimum_spanning_tree(__edges, __nodes);
    auto loops    = __find_graph_loops(__edges, __nodes, selected);
    for (const auto &loop_pair : loops) {
        GiNaC::ex sum;
        for (const auto &edge : loop_pair.first)
            sum = sum + P(edge);
        for (const auto &edge : loop_pair.second)
            sum = sum - P(edge);
        __kpl.append(sum == 0);
    }
    auto parallel_list = __find_parallel_edges(__edges);
    for (const auto &group : parallel_list) {
        for (const edge_t &edge : group.others) {
            __kpl.append(P(group.representative) == P(edge));
        }
    }
}

} // namespace symsolbin
