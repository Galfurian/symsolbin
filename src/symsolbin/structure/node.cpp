/// @file node.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include "symsolbin/structure/node.hpp"

#include <algorithm>

namespace symsolbin
{

bool collection_contains_node(const node_list_t &collection, const node_t &node)
{
    return std::find(collection.begin(), collection.end(), node) != collection.end();
}

std::ostream &operator<<(std::ostream &lhs, const node_list_t &rhs)
{
    lhs << "[";
    for (size_t i = 0; i < rhs.size(); ++i) {
        lhs << rhs[i];
        if (i != (rhs.size() - 1))
            lhs << ", ";
    }
    lhs << "]";
    return lhs;
}

} // namespace symsolbin
