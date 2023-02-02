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
    edge_t V0, R0, C0, L0, RL;
    value_t r0, c0, l0, rl;

public:
    Model()
        : in("in"),
          out("out"),
          gnd("gnd", true),

          V0(gnd, in, "V0"),
          R0(in, out, "R0"),
          C0(out, gnd, "C0"),
          L0(out, gnd, "L0"),
          RL(out, gnd, "RL"),

          r0("r0"),
          c0("c0"),
          l0("l0"),
          rl("rl")
    {
#if 0
        GiNaC::lst equations;
        equations.append(P(R0) == r0 * F(R0));
        equations.append(F(C0) == c0 * ddt(P(C0)));
        equations.append(P(RL) == rl * F(RL));

        equations.append(P(V0) + P(R0) + P(C0) == 0);
        equations.append(P(C0) == P(RL));
        
        equations.append(F(V0) == F(R0));
        equations.append(F(R0) == F(C0) + F(RL));
        GiNaC::lst unknowns;
        unknowns.append(F(V0));
        unknowns.append(P(R0));
        unknowns.append(F(R0));
        unknowns.append(P(C0));
        unknowns.append(F(C0));
        unknowns.append(P(RL));
        unknowns.append(F(RL));

        auto result = ginac_helper::split_solved(ginac_helper::solve(equations, unknowns, GiNaC::solve_algo::markowitz));
        std::cout << "result:\n";
        for (const auto &it : result)
            std::cout << "    " << it << "\n";
#endif
    }

    inline void setup() override
    {
        equations(
            P(R0) == r0 * F(R0),
            F(C0) == c0 * ddt(P(C0)),
            F(L0) == (1 / l0) * idt(P(L0)),
            P(RL) == rl * F(RL));
        unknowns(
            F(V0),
            P(R0), F(R0),
            P(C0), F(C0),
            P(L0), F(L0),
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