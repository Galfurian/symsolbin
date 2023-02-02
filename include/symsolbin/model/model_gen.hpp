/// @file   model_gen.hpp
/// @author Enrico Fraccaroli
/// @date   Sep 20, 2021

#pragma once

#include "symsolbin/solver/analog_model.hpp"

namespace symsolbin
{

/// @brief Creates a simulation code that uses dense matrices from Eigen3.
/// @param details the details about the solved system of equations.
/// @param name the name of the output class.
/// @return the generated code.
std::string generate_class_dense(const model_details_t &details, const std::string &name);

/// @brief Creates a simulation code that uses sparse matrices from Eigen3.
/// @param details the details about the solved system of equations.
/// @param name the name of the output class.
/// @return the generated code.
std::string generate_class_sparse(const model_details_t &details, const std::string &name);

} // namespace symsolbin