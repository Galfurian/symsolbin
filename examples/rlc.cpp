/// @file rlc.cpp
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
    node_t in, n1, n2, out, gnd;
    edge_t V0, R0, L0, C0, RL;
    value_t r0, l0, c0, rl;

public:
    Model()
        : in("in"),
          n1("n1"),
          n2("n2"),
          out("out"),
          gnd("gnd", true),

          V0(gnd, in, "V0"),
          R0(in, n1, "R0"),
          L0(n1, n2, "L0"),
          C0(n2, out, "C0"),
          RL(out, gnd, "RL"),

          r0("r0"),
          l0("l0"),
          c0("c0"),
          rl("rl")
    {
        // Nothing to do.
    }

    inline void setup() override
    {
        equations(
            P(R0) == r0 * F(R0),
            P(L0) == l0 * ddt(F(L0)),
            F(C0) == c0 * ddt(P(C0)),
            P(RL) == rl * F(RL));
        unknowns(
            F(V0),
            P(R0), F(R0),
            P(L0), F(L0),
            P(C0), F(C0),
            P(RL), F(RL));
        values(r0, l0, c0, rl);
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
    std::cout << generate_class(model, "rlc_t") << "\n";
    return 0;
}

//==============================================================================

#include <symsolbin/simulation/analog_pair.hpp>
#include <symsolbin/simulation/simulation.hpp>

class rlc_t {
public:
    /// Analog edges.
    analog_pair_t C0, L0, R0, RL, V0;
    /// System variables.
    analog_value_t r0, l0, c0, rl;
    /// Support variables.
    analog_value_t ddt0, ddt1;
    /// Constructor.
    rlc_t()
        : C0(), L0(), R0(), RL(), V0(),
          r0(), l0(), c0(), rl(),
          ddt0(), ddt1()
    {
    }
    void run()
    {
        // Get the system timestep.
        analog_time_t ts = _system_timestep();
        // Evaluate the analog values.
        V0.flw = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * (c0 * ts * ddt0 + c0 * ts * V0.pot - c0 * ddt1 * l0);
        R0.pot = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * r0 * (c0 * ts * ddt0 + c0 * ts * V0.pot - c0 * ddt1 * l0);
        R0.flw = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * (c0 * ts * ddt0 + c0 * ts * V0.pot - c0 * ddt1 * l0);
        L0.pot = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * (c0 * V0.pot + c0 * r0 * ddt1 + c0 * ddt0 + c0 * rl * ddt1 + ts * ddt1) * l0;
        L0.flw = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * (c0 * ts * ddt0 + c0 * ts * V0.pot - c0 * ddt1 * l0);
        C0.pot = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * ((ts * ts) * V0.pot - c0 * ts * rl * ddt0 - c0 * ddt0 * l0 - ts * ddt1 * l0 - c0 * ts * r0 * ddt0);
        C0.flw = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * (c0 * ts * ddt0 + c0 * ts * V0.pot - c0 * ddt1 * l0);
        RL.pot = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * rl * (c0 * ts * ddt0 + c0 * ts * V0.pot - c0 * ddt1 * l0);
        RL.flw = -1.0 / ((ts * ts) + c0 * l0 + c0 * ts * rl + c0 * ts * r0) * (c0 * ts * ddt0 + c0 * ts * V0.pot - c0 * ddt1 * l0);
        // Update support variables.
        ddt0 = 1.0 / ts * (C0.pot - ddt0);
        ddt1 = 1.0 / ts * (L0.flw - ddt1);
    }
};
// =============================================================================
