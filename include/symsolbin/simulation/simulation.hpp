/// @file simulation.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#pragma once

#include "symsolbin/simulation/double_op.hpp"

namespace symsolbin
{

/// Unit charge.
#define ECHARGE 1.6021766E-19
/// Boltzmann constant.
#define CBOLTZMANN 1.3806488E-23

/// Define what the analog time is.
using analog_time_t = double;
/// Define what the analog time is.
using digital_time_t = unsigned int;

/// @brief Returns the simulated time.
inline analog_time_t &_system_simulated_time()
{
    static analog_time_t simulated_time = 0.0;
    return simulated_time;
}

/// @brief Returns the current time.
inline analog_time_t &_system_abstime()
{
    static analog_time_t abstime = 0.0;
    return abstime;
}

/// @brief returns the minimum timestep.
inline analog_time_t &_system_minimum_timestep()
{
    static analog_time_t minimum_timestep = 1e-12;
    return minimum_timestep;
}

/// @brief returns the minimum timestep.
inline analog_time_t &_system_maximum_timestep()
{
    static analog_time_t maximum_timestep = 1.0;
    return maximum_timestep;
}

/// @brief returns the timestep.
inline analog_time_t &_system_timestep()
{
    static analog_time_t timestep = 0.0;
    return timestep;
}

/// @brief Returns the number of computed points.
inline unsigned long int &_system_computed_points()
{
    static unsigned long int computed_points = 0;
    return computed_points;
}

/// @brief Returns the temperature in kelvin.
inline double &_temperature()
{
    static double temperature = 300;
    return temperature;
}

/// @brief
/// @details
/// The thermal voltage depends on absolute temperature T as
///     Vt = (k * T) / q;
/// where q is the magnitude of the electrical charge on the
/// electron with a value 1.6021766208(98)×10−19 C[1] and k is the Boltzmann
/// constant, 1.380648521(79)×10−23 J/K. In electronvolts, the Boltzmann
/// constant is 8.6173303(50)×10−5 eV/K,[1] making it easy to calculate that
/// at room temperature (≈ 300 K), the value of the thermal voltage is
/// approximately 25.85 millivolts ≈ 26 mV.[8] The thermal voltage is also
/// important in plasmas and electrolyte solutions; in both cases it provides
/// a measure of how much the spatial distribution of electrons or ions is
/// affected by a boundary held at a fixed voltage.[9][10].
/// @return
inline double _thermal_voltage(double T = _temperature())
{
    return ((CBOLTZMANN * T) / ECHARGE);
}

/// @brief Checks if we completed the simulation.
/// @return true if the simulation is completed, false otherwise.
inline bool _system_simulation_completed()
{
    return is_lequal(_system_abstime(), _system_simulated_time() + _system_timestep());
}

/// @brief Advances the simulation time and checks if we completed.
/// @return true if the simulation is completed, false otherwise.
inline bool _system_advance_time()
{
    // Advance the analog time.
    _system_abstime() += _system_timestep();
    // Increment the number of computed points.
    ++_system_computed_points();
    // Check if the simulation is completed.
    return _system_simulation_completed();
}

} // namespace symsolbin
