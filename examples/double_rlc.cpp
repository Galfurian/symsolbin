/// @file double_rlc.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include <symsolbin/solver/analog_model.hpp>
#include <symsolbin/solver/ginac_helper.hpp>

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
    std::cout << "\nResult:\n";
    model.print_result();
    return 0;
}