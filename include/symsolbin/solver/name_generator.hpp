/// @file name_generator.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#pragma once

#include <string>
#include <map>

namespace symsolbin::name_gen
{

/// @brief Randomizes the name.
/// @param prefix the prefix of the name.
/// @return a random, but unique name.
static inline std::string get_name(std::string const &prefix)
{
    // Keeps track of used name.
    static std::map<std::string, bool> used;
    // We add postfix at the end of the prefix to make unique names (e.g., a1,
    // a2, a3, ...).
    long long int postfix = 0;
    // The unique lable we are generating.
    std::string label = prefix + std::to_string(postfix);
    // Keep generating label until we find an unused one.
    while (used[label]) {
        label = prefix + std::to_string(postfix++);
    }
    // Set the label as used.
    used[label] = true;
    return label;
}

} // namespace symsolbin::name_gen
