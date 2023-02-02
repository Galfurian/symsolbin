/// @file node.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @copyright (c) 2014-2023 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#pragma once

#include <ostream>
#include <string>
#include <vector>

namespace symsolbin
{

/// @brief An analog node.
class node_t {
public:
    /// @brief Construct a new node_t object.
    /// @param name the name of the node.
    /// @param ground the node is a ground node.
    node_t(std::string name, bool ground = false)
        : _name(name),
          _ground(ground)
    {
        // Nothing to do.
    }

    node_t(const node_t &other) = default;

    node_t &operator=(const node_t &other) = default;

    ~node_t() = default;

    /// @brief Sets the name.
    /// @param name the name of the node.
    /// @return a reference to this object.
    inline node_t &set_name(std::string name)
    {
        _name = name;
        return *this;
    }

    /// @brief Sets the node as either ground or normal node.
    /// @param ground if the node is a ground node.
    /// @return a reference to this object.
    inline node_t &set_ground(bool ground)
    {
        _ground = ground;
        return *this;
    }

    /// @brief Returns the name of the node.
    /// @return the name.
    inline std::string get_name() const
    {
        return _name;
    }

    /// @brief Returns if the node is a ground node.
    /// @return the ground status.
    inline bool is_ground() const
    {
        return _ground;
    }

    /// @brief Checks equality between nodes.
    /// @param rhs the other node.
    /// @return true if they are the same node, false otherwise.
    inline bool operator==(const node_t &rhs) const
    {
        return _name == rhs._name;
    }

    /// @brief Checks inequality between nodes.
    /// @param rhs the other node.
    /// @return true if they are different node, false otherwise.
    inline bool operator!=(const node_t &rhs) const
    {
        return _name != rhs._name;
    }

    /// @brief Compares the names of two nodes, useful for sorting nodes inside a collection.
    inline bool operator<(const node_t &rhs) const
    {
        return _name < rhs._name;
    }

    /// @brief Stream operator.
    inline friend std::ostream &operator<<(std::ostream &lhs, const node_t &rhs)
    {
        if (rhs._ground)
            lhs << "[" << rhs._name << "]";
        else
            lhs << rhs._name;
        return lhs;
    }

private:
    /// Name of the node.
    std::string _name;
    /// Is this a reference node.
    bool _ground;
};

/// A list of nodes.
using node_list_t = std::vector<node_t>;

/// @brief Checks if the collection contains the given node.
/// @param collection the collection of nodes.
/// @param node the node we are looking for.
/// @return true if it is contained, false otherwise.
bool collection_contains_node(const node_list_t &collection, const node_t &node);

/// @brief Stream operator for a list of nodes.
std::ostream &operator<<(std::ostream &lhs, const node_list_t &rhs);

} // namespace symsolbin
