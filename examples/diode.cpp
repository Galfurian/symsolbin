/// @file diode.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include <symsolbin/solver/analog_model.hpp>
#include <symsolbin/solver/ginac_helper.hpp>
#include <symsolbin/model/model_gen.hpp>

using namespace symsolbin;
using namespace symsolbin::ginac_helper;

/// @brief
class Model : public analog_model_t {
private:
    node_t in, out, gnd;
    edge_t V0, D0, RL;

    value_t vin, rd0, rl, phi, rf;

public:
    Model()
        : in("in"),
          out("out"),
          gnd("gnd", true),

          V0(gnd, in, "V0"),
          D0(in, out, "D0"),
          RL(out, gnd, "RL"),

          vin("vin"),
          rd0("rd0"),
          rl("rl"),
          phi("phi"),
          rf("rf")
    {
        // Nothing to do.
    }

    inline void setup() override
    {
        equations(
            // Source.
            P(V0) == vin,
            // Model
            F(D0) == (P(D0) - phi) / rf,
            // Load resistor
            P(RL) == F(RL) * rl);
        unknowns(
            P(V0), F(V0),
            P(D0), F(D0),
            P(RL), F(RL));
        values(vin, rd0, rl, phi, rf);
    }
};

int main(int, char *[])
{
    Model model;
    // ts.set_value(1e-03);
    model.run_solver();
    std::cout << "\n";
    std::cout << model << "\n";
    std::cout << "\n";
    std::cout << generate_class(model, "diode_t") << "\n";
    return 0;
}

//==============================================================================
#include <symsolbin/simulation/analog_pair.hpp>
#include <symsolbin/simulation/simulation.hpp>

class diode_t {
public:
    /// Analog edges.
    analog_pair_t D0, RL, V0;
    /// System variables.
    analog_value_t vin, rd0, rl, phi, rf;
    /// Constructor.
    diode_t()
        : D0(), RL(), V0(),
          vin(), rd0(), rl(), phi(), rf()
    {
    }
    void run()
    {
        // Get the system timestep.
        analog_time_t ts = _system_timestep();
        // Evaluate the analog values.
        V0.pot = vin;
        if (V0.pot > phi) {
            V0.flw = -(phi + vin) / (rl + rf);
            D0.pot = -1.0 / (rl + rf) * (vin * rf - rl * phi);
            D0.flw = -(phi + vin) / (rl + rf);
            RL.pot = -(phi + vin) * rl / (rl + rf);
            RL.flw = -(phi + vin) / (rl + rf);
        } else{
            V0.flw = 0;
            D0.pot = V0.pot;
            D0.flw = 0;
            RL.pot = 0;
            RL.flw = 0;
        }
    }
};
// =============================================================================
