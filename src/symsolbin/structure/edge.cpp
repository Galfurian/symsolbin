/// @file edge.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include "symsolbin/structure/edge.hpp"

#include <algorithm>

namespace symsolbin
{

bool collection_contains_edge(const edge_list_t &collection, const edge_t &edge)
{
    return std::find(collection.begin(), collection.end(), edge) != collection.end();
}

std::ostream &operator<<(std::ostream &lhs, const edge_list_t &rhs)
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
