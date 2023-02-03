/// @file not.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#include <symsolbin/solver/analog_model.hpp>
#include <symsolbin/solver/ginac_helper.hpp>

using namespace symsolbin;
using namespace symsolbin::ginac_helper;

/// @brief
class Model : public analog_model_t {
private:
    node_t q, a, vdd, vss, gnd;
    edge_t V0, RL0, RL1, RL2;
    edge_t C0, C1, R0, R1, C2, C3;
    value_t c0, c1, r0, r1, c2, c3;

public:
    Model()
        : q("q"),
          a("a"),
          vdd("vdd"),
          vss("vss"),
          gnd("gnd", true),

          V0(gnd, a, "V0"),
          RL0(q, gnd, "RL0"),
          RL1(vdd, gnd, "RL1"),
          RL2(vss, gnd, "RL2"),

          C0(a, vdd, "C0"),
          C1(a, vss, "C1"),
          R0(vdd, q, "R0"),
          R1(vss, q, "R1"),
          C2(vdd, q, "C2"),
          C3(vss, q, "C3"),

          c0("c0"),
          c1("c1"),
          r0("r0"),
          r1("r1"),
          c2("c2"),
          c3("c3")
    {
        // Nothing to do.
    }

    inline void setup() override
    {
        equations(
            F(C0) == ddt(P(C0)) * c0,
            F(C1) == ddt(P(C1)) * c1,
            F(R0) == P(R0) / r0,
            F(R1) == P(R1) / r1,
            F(C2) == ddt(P(C2)) * c2,
            F(C3) == ddt(P(C3)) * c3,
            F(RL0) == P(RL0) / 1e09,
            F(RL1) == P(RL1) / 1e09,
            F(RL2) == P(RL2) / 1e09);
        unknowns(
            F(V0),
            P(C0), F(C0),
            P(C1), F(C1),
            P(R0), F(R0),
            P(R1), F(R1),
            P(C2), F(C2),
            P(C3), F(C3),
            P(RL0), F(RL0),
            P(RL1), F(RL1),
            P(RL2), F(RL2));
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