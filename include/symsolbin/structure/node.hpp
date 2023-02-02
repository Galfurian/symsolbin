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
struct node_t {
    /// Name of the node.
    std::string name;
    /// Is this a reference node.
    bool ground;

    /// @brief Construct a new node_t object.
    /// @param _name the name of the node.
    node_t(std::string _name, bool _ground = false)
        : name(_name),
          ground(_ground)
    {
        // Nothing to do.
    }

    node_t(const node_t &other) = default;

    node_t &operator=(const node_t &other) = default;

    ~node_t() = default;

    inline bool operator==(const node_t &rhs) const
    {
        return name == rhs.name;
    }

    inline bool operator!=(const node_t &rhs) const
    {
        return name != rhs.name;
    }

    inline bool operator<(const node_t &rhs) const
    {
        return name < rhs.name;
    }

    inline friend std::ostream &operator<<(std::ostream &lhs, const node_t &rhs)
    {
        if (rhs.ground)
            lhs << "[" << rhs.name << "]";
        else
            lhs << rhs.name;
        return lhs;
    }
};

/// A list of nodes.
using node_list_t = std::vector<node_t>;

bool collection_contains_node(const node_list_t &collection, const node_t &node);

std::ostream &operator<<(std::ostream &lhs, const node_list_t &rhs);

} // namespace symsolbin
