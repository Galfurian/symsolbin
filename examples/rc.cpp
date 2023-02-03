/// @file rc.cpp
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
    edge_t V0, R0, C0;
    value_t vin, r0, c0;

public:
    Model()
        : in("in"),
          out("out"),
          gnd("gnd", true),

          V0(gnd, in, "V0"),
          R0(in, out, "R0"),
          C0(out, gnd, "C0"),

          vin("vin"),
          r0("r0"),
          c0("c0")
    {
        // Nothing to do.
    }

    inline void setup() override
    {
        equations(
            P(V0) == vin,
            P(R0) == r0 * F(R0),
            F(C0) == c0 * ddt(P(C0)));
        unknowns(
            P(V0), F(V0),
            P(R0), F(R0),
            P(C0), F(C0));
        values(vin, r0, c0);
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
    std::cout << generate_class(model, "rc_t") << "\n";
    return 0;
}

//==============================================================================

#include <symsolbin/simulation/analog_pair.hpp>
#include <symsolbin/simulation/simulation.hpp>

class rc_t {
public:
    /// Analog edges.
    analog_pair_t C0, R0, V0;
    /// System variables.
    analog_value_t vin, r0, c0;
    /// Support variables.
    analog_value_t ddt0;
    /// Constructor.
    rc_t()
        : C0(), R0(), V0(),
          vin(), r0(), c0(),
          ddt0()
    {
    }
    void run()
    {
        // Get the system timestep.
        analog_time_t ts = _system_timestep();
        // Evaluate the analog values.
        V0.pot = vin;
        V0.flw = -(vin + ddt0) * c0 / (r0 * c0 + ts);
        R0.pot = -(vin + ddt0) * r0 * c0 / (r0 * c0 + ts);
        R0.flw = -(vin + ddt0) * c0 / (r0 * c0 + ts);
        C0.pot = (ddt0 * r0 * c0 - vin * ts) / (r0 * c0 + ts);
        C0.flw = -(vin + ddt0) * c0 / (r0 * c0 + ts);
        // Update support variables.
        ddt0 = -(ddt0 - C0.pot) / ts;
    }
};
// =============================================================================
