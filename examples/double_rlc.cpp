/// @file double_rlc.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include <symsolbin/solver/analog_model.hpp>
#include <symsolbin/solver/ginac_helper.hpp>
#include <symsolbin/model/model_gen.hpp>

using namespace symsolbin;
using namespace symsolbin::ginac_helper;

/// @brief
class Model : public analog_model_t {
public:
    node_t n0, n1, n2, n3, n4, gnd;
    edge_t V0, R0, L0, C0, R1, L1, C1;
    value_t r0, c0, l0, r1, c1, l1, vin;

    Model()
        : n0("n0"),
          n1("n1"),
          n2("n2"),
          n3("n3"),
          n4("n4"),
          gnd("gnd", true),

          V0(gnd, n0, "V0"),
          R0(n0, n1, "R0"),
          L0(n1, n2, "L0"),
          C0(n2, n3, "C0"),
          R1(n4, gnd, "R1"),
          L1(n3, gnd, "L1"),
          C1(n3, n4, "C1"),

          r0("r0"),
          c0("c0"),
          l0("l0"),
          r1("r1"),
          c1("c1"),
          l1("l1"),
          vin("vin")
    {
        // Nothing to do.
    }

    inline void setup() override
    {
        equations(
            P(V0) == vin,
            P(R0) == r0 * F(R0),
            P(L0) == l0 * ddt(F(L0)),
            P(C0) == (1 / c0) * idt(F(C0)),
            P(R1) == r1 * F(R1),
            P(L1) == l1 * ddt(F(L1)),
            P(C1) == (1 / c1) * idt(F(C1)));
        unknowns(
            P(V0), F(V0),
            P(R0), F(R0),
            P(L0), F(L0),
            P(C0), F(C0),
            P(R1), F(R1),
            P(L1), F(L1),
            P(C1), F(C1));
        values(r0, c0, l0, r1, c1, l1, vin);
    }
};

int main(int, char *[])
{
    Model model;
    // model.vin.set_value(1);
    // model.r0.set_value(1);
    // model.c0.set_value(1);
    // model.l0.set_value(1);
    // model.r1.set_value(1);
    // model.c1.set_value(1);
    // model.l1.set_value(1);
    // ts.set_value(1e-03);
    model.run_solver();
    std::cout << "\n";
    std::cout << model << "\n";
    std::cout << "\n";
    std::cout << generate_class(model, "double_rlc_t") << "\n";
    return 0;
}

//==============================================================================

#include <symsolbin/simulation/analog_pair.hpp>
#include <symsolbin/simulation/simulation.hpp>

class double_rlc_t {
public:
    /// Analog edges.
    analog_pair_t C0, C1, L0, L1, R0, R1, V0;
    /// System variables.
    analog_value_t r0, c0, l0, r1, c1, l1, vin;
    /// Support variables.
    analog_value_t idt0, ddt0, idt1, ddt1;
    /// Constructor.
    double_rlc_t()
        : C0(), C1(), L0(), L1(), R0(), R1(), V0(),
          r0(), c0(), l0(), r1(), c1(), l1(), vin(),
          idt0(), ddt0(), idt1(), ddt1()
    {
    }
    void run()
    {
        // Get the system timestep.
        analog_time_t ts = _system_timestep();
        // Evaluate the analog values.
        V0.pot = vin;
        V0.flw = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (ddt1 * c1 * c0 * l1 * l0 + r1 * ddt1 * c1 * c0 * l0 * ts + r1 * c1 * c0 * ddt0 * l1 * ts - c0 * idt0 * l1 * ts - c1 * c0 * l1 * ts * vin - idt1 * c1 * l1 * ts - r1 * idt1 * c1 * (ts * ts) + c0 * ddt0 * l1 * (ts * ts) - r1 * c1 * c0 * (ts * ts) * vin - c0 * (ts * ts * ts) * vin + ddt1 * c0 * l0 * (ts * ts) - idt1 * (ts * ts * ts));
        R0.pot = r0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (ddt1 * c1 * c0 * l1 * l0 + r1 * ddt1 * c1 * c0 * l0 * ts + r1 * c1 * c0 * ddt0 * l1 * ts - c0 * idt0 * l1 * ts - c1 * c0 * l1 * ts * vin - idt1 * c1 * l1 * ts - r1 * idt1 * c1 * (ts * ts) + c0 * ddt0 * l1 * (ts * ts) - r1 * c1 * c0 * (ts * ts) * vin - c0 * (ts * ts * ts) * vin + ddt1 * c0 * l0 * (ts * ts) - idt1 * (ts * ts * ts));
        R0.flw = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (ddt1 * c1 * c0 * l1 * l0 + r1 * ddt1 * c1 * c0 * l0 * ts + r1 * c1 * c0 * ddt0 * l1 * ts - c0 * idt0 * l1 * ts - c1 * c0 * l1 * ts * vin - idt1 * c1 * l1 * ts - r1 * idt1 * c1 * (ts * ts) + c0 * ddt0 * l1 * (ts * ts) - r1 * c1 * c0 * (ts * ts) * vin - c0 * (ts * ts * ts) * vin + ddt1 * c0 * l0 * (ts * ts) - idt1 * (ts * ts * ts));
        L0.pot = -1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (idt1 * l0 * (ts * ts) + ddt1 * r0 * c0 * l0 * (ts * ts) + c0 * l0 * (ts * ts) * vin + r1 * ddt1 * r0 * c1 * c0 * l0 * ts + idt1 * c1 * l1 * l0 - c0 * ddt0 * l1 * l0 * ts + r1 * c1 * c0 * l0 * ts * vin + ddt1 * c1 * l1 * l0 * ts + c0 * idt0 * l1 * l0 + r1 * ddt1 * c1 * c0 * l1 * l0 + r1 * idt1 * c1 * l0 * ts + ddt1 * r0 * c1 * c0 * l1 * l0 + ddt1 * c0 * l1 * l0 * ts + ddt1 * l0 * (ts * ts * ts) - r1 * c1 * c0 * ddt0 * l1 * l0 + c1 * c0 * l1 * l0 * vin + r1 * ddt1 * c1 * l0 * (ts * ts));
        L0.flw = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (ddt1 * c1 * c0 * l1 * l0 + r1 * ddt1 * c1 * c0 * l0 * ts + r1 * c1 * c0 * ddt0 * l1 * ts - c0 * idt0 * l1 * ts - c1 * c0 * l1 * ts * vin - idt1 * c1 * l1 * ts - r1 * idt1 * c1 * (ts * ts) + c0 * ddt0 * l1 * (ts * ts) - r1 * c1 * c0 * (ts * ts) * vin - c0 * (ts * ts * ts) * vin + ddt1 * c0 * l0 * (ts * ts) - idt1 * (ts * ts * ts));
        C0.pot = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (idt1 * l0 * (ts * ts) + r0 * idt1 * (ts * ts * ts) + r1 * r0 * idt1 * c1 * (ts * ts) + r0 * idt1 * c1 * l1 * ts + ddt0 * l1 * (ts * ts * ts) + idt1 * l1 * (ts * ts) - ((ts * ts) * (ts * ts)) * vin + idt1 * c1 * l1 * l0 + r1 * c1 * ddt0 * l1 * (ts * ts) - c1 * l1 * (ts * ts) * vin + ddt1 * c1 * l1 * l0 * ts - r1 * c1 * (ts * ts * ts) * vin + r1 * idt1 * c1 * l0 * ts + ddt1 * l0 * (ts * ts * ts) + r1 * idt1 * c1 * l1 * ts + r1 * ddt1 * c1 * l0 * (ts * ts) - idt0 * l1 * (ts * ts));
        C0.flw = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (ddt1 * c1 * c0 * l1 * l0 + r1 * ddt1 * c1 * c0 * l0 * ts + r1 * c1 * c0 * ddt0 * l1 * ts - c0 * idt0 * l1 * ts - c1 * c0 * l1 * ts * vin - idt1 * c1 * l1 * ts - r1 * idt1 * c1 * (ts * ts) + c0 * ddt0 * l1 * (ts * ts) - r1 * c1 * c0 * (ts * ts) * vin - c0 * (ts * ts * ts) * vin + ddt1 * c0 * l0 * (ts * ts) - idt1 * (ts * ts * ts));
        R1.pot = r1 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (ddt1 * c1 * c0 * l1 * l0 - c1 * ddt0 * l1 * (ts * ts) - c0 * idt0 * l0 * ts - idt0 * (ts * ts * ts) - c1 * c0 * ddt0 * l1 * l0 - c0 * idt0 * l1 * ts - c1 * c0 * l1 * ts * vin - idt1 * c1 * l1 * ts - r0 * c0 * idt0 * (ts * ts) - r0 * c1 * c0 * ddt0 * l1 * ts);
        R1.flw = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (ddt1 * c1 * c0 * l1 * l0 - c1 * ddt0 * l1 * (ts * ts) - c0 * idt0 * l0 * ts - idt0 * (ts * ts * ts) - c1 * c0 * ddt0 * l1 * l0 - c0 * idt0 * l1 * ts - c1 * c0 * l1 * ts * vin - idt1 * c1 * l1 * ts - r0 * c0 * idt0 * (ts * ts) - r0 * c1 * c0 * ddt0 * l1 * ts);
        L1.pot = -(ddt0 * l1 * (ts * ts * ts) + idt1 * l1 * (ts * ts) + c0 * ddt0 * l1 * l0 * ts + r1 * c1 * ddt0 * l1 * (ts * ts) + r0 * c0 * ddt0 * l1 * (ts * ts) - c0 * idt0 * l1 * l0 - r1 * ddt1 * c1 * c0 * l1 * l0 - ddt1 * c0 * l1 * l0 * ts + c0 * l1 * (ts * ts) * vin + r1 * c1 * c0 * ddt0 * l1 * l0 + r1 * idt1 * c1 * l1 * ts - r0 * c0 * idt0 * l1 * ts + r1 * c1 * c0 * l1 * ts * vin - idt0 * l1 * (ts * ts) + r1 * r0 * c1 * c0 * ddt0 * l1 * ts) / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts));
        L1.flw = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (c1 * ddt0 * l1 * (ts * ts) + c0 * idt0 * l0 * ts + r1 * ddt1 * c1 * c0 * l0 * ts + r1 * c1 * c0 * ddt0 * l1 * ts + idt0 * (ts * ts * ts) + c1 * c0 * ddt0 * l1 * l0 - r1 * idt1 * c1 * (ts * ts) + r0 * c0 * idt0 * (ts * ts) + c0 * ddt0 * l1 * (ts * ts) - r1 * c1 * c0 * (ts * ts) * vin - c0 * (ts * ts * ts) * vin + ddt1 * c0 * l0 * (ts * ts) + r0 * c1 * c0 * ddt0 * l1 * ts - idt1 * (ts * ts * ts));
        C1.pot = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (r1 * c0 * idt0 * l1 * ts - ddt0 * l1 * (ts * ts * ts) - idt1 * l1 * (ts * ts) - c0 * ddt0 * l1 * l0 * ts + r1 * c0 * idt0 * l0 * ts - r0 * c0 * ddt0 * l1 * (ts * ts) + c0 * idt0 * l1 * l0 + ddt1 * c0 * l1 * l0 * ts - c0 * l1 * (ts * ts) * vin + r1 * idt0 * (ts * ts * ts) + r0 * c0 * idt0 * l1 * ts + idt0 * l1 * (ts * ts) + r1 * r0 * c0 * idt0 * (ts * ts));
        C1.flw = 1.0 / (c1 * c0 * l1 * l0 + r1 * r0 * c1 * c0 * (ts * ts) + ((ts * ts) * (ts * ts)) + r0 * c0 * (ts * ts * ts) + r0 * c1 * c0 * l1 * ts + r1 * c1 * c0 * l1 * ts + r1 * c1 * (ts * ts * ts) + c0 * l1 * (ts * ts) + c1 * l1 * (ts * ts) + r1 * c1 * c0 * l0 * ts + c0 * l0 * (ts * ts)) * (ddt1 * c1 * c0 * l1 * l0 - c1 * ddt0 * l1 * (ts * ts) - c0 * idt0 * l0 * ts - idt0 * (ts * ts * ts) - c1 * c0 * ddt0 * l1 * l0 - c0 * idt0 * l1 * ts - c1 * c0 * l1 * ts * vin - idt1 * c1 * l1 * ts - r0 * c0 * idt0 * (ts * ts) - r0 * c1 * c0 * ddt0 * l1 * ts);
        // Update support variables.
        idt0 = C1.flw * ts;
        ddt0 = 1.0 / ts * (L1.flw - ddt0);
        idt1 = ts * C0.flw;
        ddt1 = -1.0 / ts * (ddt1 - L0.flw);
    }
};
// =============================================================================
