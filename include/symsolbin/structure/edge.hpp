/// @file edge.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @copyright (c) 2014-2023 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#pragma once

#include "node.hpp"

namespace symsolbin
{

/// @brief An analog edge.
struct edge_t {
    ///
    node_t n1;
    ///
    node_t n2;
    /// Name of the edge.
    std::string alias;

    /// @brief Construct a new edge_t object.
    /// @param _name the name of the edge.
    edge_t(const node_t &_n1, const node_t &_n2, std::string _alias = "")
        : n1(_n1),
          n2(_n2),
          alias(_alias)
    {
        if (alias.empty())
            alias = _n1.name + "_" + _n2.name;
    }

    inline bool is_connected_to(const node_t &node) const
    {
        return (n1 == node) || (n2 == node);
    }

    inline bool is_parallel_to(const edge_t &other) const
    {
        return ((n1 == other.n1) && (n2 == other.n2)) ||
               ((n2 == other.n1) && (n1 == other.n2));
    }

    inline bool not_connected_to(const edge_t &other) const
    {
        return ((n1 != other.n1) || (n2 != other.n2)) &&
               ((n2 != other.n1) || (n1 != other.n2));
    }

    inline bool operator==(const edge_t &rhs) const
    {
        return alias == rhs.alias;
    }

    inline bool operator!=(const edge_t &rhs) const
    {
        return alias != rhs.alias;
    }

    inline bool operator<(const edge_t &rhs) const
    {
        return alias < rhs.alias;
    }

    /// @brief Stream operator for an edge.
    inline friend std::ostream &operator<<(std::ostream &lhs, const edge_t &rhs)
    {
        lhs << rhs.alias;
        return lhs;
    }
};

/// A list of edges.
using edge_list_t = std::vector<edge_t>;

/// @brief Checks if the collection contains the given edge.
/// @param collection the collection of edges.
/// @param edge the edge we are looking for.
/// @return true if it is contained, false otherwise.
bool collection_contains_edge(const edge_list_t &collection, const edge_t &edge);

/// @brief Stream operator for a list of edges.
std::ostream &operator<<(std::ostream &lhs, const edge_list_t &rhs);

} // namespace symsolbin