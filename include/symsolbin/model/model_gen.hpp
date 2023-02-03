/// @file   model_gen.hpp
/// @author Enrico Fraccaroli
/// @date   Sep 20, 2021

#pragma once

#include "symsolbin/solver/analog_model.hpp"

namespace symsolbin
{

/// @brief Creates a C++ simulation code.
/// @param model the analog model we want to print.
/// @param name the name of the output class.
/// @return the generated code.
std::string generate_class(const analog_model_t &model, const std::string &name);

/// @brief Creates a simulation code that uses dense matrices from Eigen3.
/// @param model the analog model we want to print.
/// @param name the name of the output class.
/// @return the generated code.
std::string generate_class_dense(const analog_model_t &model, const std::string &name);

/// @brief Creates a simulation code that uses sparse matrices from Eigen3.
/// @param model the analog model we want to print.
/// @param name the name of the output class.
/// @return the generated code.
std::string generate_class_sparse(const analog_model_t &model, const std::string &name);

} // namespace symsolbin