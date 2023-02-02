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
class edge_t {
public:
    /// @brief Construct a new edge_t object.
    /// @param _n1 the first node.
    /// @param _n2 the second node.
    /// @param _alias the name of the edge.
    edge_t(const node_t &_n1, const node_t &_n2, std::string _alias = "")
        : n1(_n1),
          n2(_n2),
          alias(_alias)
    {
        if (alias.empty())
            alias = _n1.get_name() + "_" + _n2.get_name();
    }

    edge_t(const edge_t &other) = default;

    edge_t &operator=(const edge_t &other) = default;

    ~edge_t() = default;

    /// @brief Returns a copy of the first node.
    inline node_t &get_first()
    {
        return n1;
    }

    /// @brief Returns a copy of the first node.
    inline const node_t &get_first() const
    {
        return n1;
    }

    /// @brief Returns a copy of the second node.
    inline node_t &get_second()
    {
        return n2;
    }

    /// @brief Returns a copy of the second node.
    inline const node_t &get_second() const
    {
        return n2;
    }

    /// @brief Returns a copy of the alias of the edge.
    inline std::string get_alias() const
    {
        return alias;
    }

    /// @brief Checks if the given node is connected to this edge.
    /// @param node the node we want to analyze.
    /// @return true if either the first or second node of this edge is the one
    /// we received, false otherwise.
    inline bool is_connected_to(const node_t &node) const
    {
        return (n1 == node) || (n2 == node);
    }

    /// @brief Checks if this edge is parallel to the one provied.
    /// @param other the other edge.
    /// @return true if they are parallel, false otherwise.
    inline bool is_parallel_to(const edge_t &other) const
    {
        return ((n1 == other.n1) && (n2 == other.n2)) || ((n2 == other.n1) && (n1 == other.n2));
    }

    /// @brief Checks if this edge is not parallel to the one provied.
    /// @param other the other edge.
    /// @return true if they are not parallel, false otherwise.
    inline bool is_not_parallel_to(const edge_t &other) const
    {
        return (n1 != other.n1) || (n1 != other.n2) || (n2 != other.n2) || (n2 != other.n2);
    }

    /// @brief Checks if this edge is connected to the one provied.
    /// @param other the other edge.
    /// @return true if they are connected, false otherwise.
    inline bool is_connected_to(const edge_t &other) const
    {
        return (n1 == other.n1) || (n1 == other.n2) || (n2 == other.n2) || (n2 == other.n2);
    }

    /// @brief Checks if this edge is not connected to the one provied.
    /// @param other the other edge.
    /// @return true if they are not connected, false otherwise.
    inline bool not_connected_to(const edge_t &other) const
    {
        return (n1 != other.n1) && (n1 != other.n2) && (n2 != other.n2) && (n2 != other.n2);
    }

    /// @brief Checks if two edges are the same.
    /// @param rhs the other edge.
    /// @return true if they are the same, false otherwise.
    inline bool operator==(const edge_t &rhs) const
    {
        return alias == rhs.alias;
    }

    /// @brief Checks if two edges are not the same.
    /// @param rhs the other edge.
    /// @return true if they are not the same, false otherwise.
    inline bool operator!=(const edge_t &rhs) const
    {
        return alias != rhs.alias;
    }

    /// @brief Compares the names of two nodes, useful for sorting nodes inside a collection.
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

private:
    /// First node.
    node_t n1;
    /// Second node.
    node_t n2;
    /// Name of the edge.
    std::string alias;
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