/// @file gen_class.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include "symsolbin/model/model_gen.hpp"

#include <cassert>

namespace symsolbin
{

template <typename List>
static inline std::string __print_list_with_commas(
    const List &lst,
    std::size_t size,
    std::string prefix  = std::string(),
    std::string postfix = std::string())
{
    std::stringstream ss;
    for (unsigned i = 0; i < size; ++i) {
        ss << prefix << lst[i] << postfix;
        if (i < (size - 1))
            ss << ", ";
    }
    return ss.str();
}

std::string generate_class(const analog_model_t &model, const std::string &name)
{
    std::stringstream ss;
    GiNaC::csrc_double(ss);

    auto structure = model.get_structure();
    auto solution  = model.get_solution();
    auto system    = model.get_system();

    std::sort(structure.edges.begin(), structure.edges.end());
    std::sort(system.values.begin(), system.values.end());
    std::sort(solution.values.begin(), solution.values.end());

    ss << "//" << std::string(78, '=') << "\n";
    ss << "\n";
    ss << "#include <symsolbin/simulation/analog_pair.hpp>\n";
    ss << "#include <symsolbin/simulation/simulation.hpp>\n";
    ss << "\n";
    ss << "class " << name << " {\n";
    ss << "public:\n";
    ss << "    /// Analog edges.\n";
    ss << "    analog_pair_t " << __print_list_with_commas(structure.edges, structure.edges.size()) << ";\n";
    ss << "    /// System variables.\n";
    ss << "    analog_value_t " << __print_list_with_commas(system.values, system.values.size()) << ";\n";
    ss << "    /// Support variables.\n";
    ss << "    analog_value_t " << __print_list_with_commas(solution.values, solution.values.size()) << ";\n";
    ss << "    /// Constructor.\n";
    ss << "    " << name << "() :\n";
    ss << "        " << __print_list_with_commas(structure.edges, structure.edges.size(), "", "()") << ",\n";
    ss << "        " << __print_list_with_commas(system.values, system.values.size(), "", "()") << ",\n";
    ss << "        " << __print_list_with_commas(solution.values, solution.values.size(), "", "()") << "\n";
    ss << "    {\n";
    ss << "    }\n";
    ss << "    void run() {\n";
    ss << "        // Get the system timestep.\n";
    ss << "        analog_time_t ts = _system_timestep();\n";
    ss << "        // Evaluate the analog values.\n";
    for (auto equation : solution.equations) {
        ss << "        " << equation.lhs() << " = " << equation.rhs() << ";\n";
    }
    ss << "        // Update support variables.\n";
    for (auto equation : solution.support) {
        ss << "        " << equation.lhs() << " = " << equation.rhs() << ";\n";
    }
    ss << "    }\n";
    ss << "};\n";
    ss << "// " << std::string(77, '=') << "\n\n";
    return ss.str();
}

} // namespace symsolbin