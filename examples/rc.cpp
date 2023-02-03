/// @file rc.cpp
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