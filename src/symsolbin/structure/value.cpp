/// @file value.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include "symsolbin/structure/value.hpp"
#include "symsolbin/solver/ginac_helper.hpp"

#include <algorithm>

namespace symsolbin
{

value_t::value_t(std::string name, double value, bool replace)
    : _symbol(GiNaC::symbol(ginac_helper::get_symbol(name))),
      _value(value),
      _replace(replace)
{
    // Nothing to do.
}

bool collection_contains_value(const value_list_t &collection, const value_t &value)
{
    return std::find(collection.begin(), collection.end(), value) != collection.end();
}

std::ostream &operator<<(std::ostream &lhs, const value_list_t &rhs)
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
