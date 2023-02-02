/// @file   model_gen.hpp
/// @author Enrico Fraccaroli
/// @date   Sep 20, 2021

#pragma once

#include "symsolbin/solver/analog_model.hpp"

namespace symsolbin
{

std::string generate_class_dense(const model_details_t &details, const std::string &name);

std::string generate_class_sparse(const model_details_t &details, const std::string &name);

} // namespace symsolbin