/// @file rlc.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include <symsolbin/solver/analog_model.hpp>
#include <symsolbin/solver/ginac_helper.hpp>

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
    }
};

int main(int, char *[])
{
    Model model;
    // ts.set_value(1e-03);
    model.run_solver();
    std::cout << "\n";
    std::cout << model << "\n";
    return 0;
}