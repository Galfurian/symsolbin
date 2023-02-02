/// @file diode.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include <symsolbin/solver/analog_model.hpp>
#include <symsolbin/solver/ginac_helper.hpp>

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
    }
};

int main(int, char *[])
{
    Model model;
    // ts.set_value(1e-03);
    model.run_solver();
    std::cout << "\n";
    model.print_result();
    std::cout << "\n";
    return 0;
}