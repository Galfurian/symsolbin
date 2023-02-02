/// @file name_generator.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#pragma once

#include <string>
#include <map>

namespace symsolbin
{

/// @brief Generates a unique label.
class name_generator_t {
public:
    static std::string get(std::string const &prefix)
    {
        static std::map<std::string, bool> used;
        long long int postfix = 0;
        std::string label     = prefix + std::to_string(postfix);
        while (used[label]) {
            label = prefix + std::to_string(postfix++);
        }
        used[label] = true;
        return label;
    }

private:
    name_generator_t() = default;

    ~name_generator_t() = default;
};

} // namespace symsolbin
