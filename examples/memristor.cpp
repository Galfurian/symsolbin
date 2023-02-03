/// @file memristor.cpp
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
    edge_t V0, M0, RL;

    value_t vin, G, rl;

public:
    Model()
        : in("in"),
          out("out"),
          gnd("gnd", true),

          V0(gnd, in, "V0"),
          M0(in, out, "M0"),
          RL(out, gnd, "RL"),

          vin("vin"),
          G("G"),
          rl("rl")
    {
        // Nothing to do.
    }

    inline void setup() override
    {
        equations(
            // Source.
            P(V0) == vin,
            // Model
            F(M0) == -G * P(M0),
            // Load resistor
            P(RL) == F(RL) * rl);
        unknowns(
            P(V0), F(V0),
            P(M0), F(M0),
            P(RL), F(RL));
        values(vin, G, rl);
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
    std::cout << generate_class(model, "memristor_t") << "\n";
    return 0;
}

//==============================================================================

#include <symsolbin/simulation/analog_pair.hpp>
#include <symsolbin/simulation/simulation.hpp>

#define X_BORDER_BUMP 10e-18

class memristor_t {
public:
    /// Analog edges.
    analog_pair_t M0, RL, V0;
    /// System variables.
    analog_value_t vin, G, rl;

    // Parameters.
    const analog_value_t Roff;
    const analog_value_t Ron;
    const analog_value_t Rinit;
    const analog_value_t D;
    const analog_value_t uv;
    const analog_value_t p_coeff;

    // local variables that should persist over time steps
    analog_value_t w_last;
    analog_value_t time_last;

    // local variables that hold temporary values
    analog_value_t window_function;
    analog_value_t w;
    analog_value_t dw;
    analog_value_t R;
    analog_value_t direction;
    analog_value_t current;
    analog_value_t time;
    analog_value_t time_delta;

    /// Constructor.
    memristor_t()
        : M0(), RL(), V0(),
          vin(), G(), rl(),
          Roff(16000),
          Ron(100),
          Rinit(11000),
          D(10e-09),
          uv(10e-15),
          p_coeff(1.0),
          window_function(),
          w(),
          dw(),
          R(),
          direction(),
          current(),
          time(),
          time_delta()
    {
        w_last    = ((Roff - Rinit) / (Roff - Ron)) * D;
        time_last = 0;
    }

    void run()
    {
        // calculate conductance
        G       = 1 / (Ron * w_last / D + Roff * (1 - w_last / D));
        current = G * M0.pot;

        direction = 0;
        if (current > 0) {
            if (w_last <= 0) {
                direction = 1;
            }
        } else {
            if (w_last >= D) {
                direction = -1;
            }
        }

        time            = _system_abstime();
        time_delta      = time - time_last;
        window_function = (1.0 - (pow(2 * w_last / D - 1, 2 * p_coeff)));
        dw              = uv * Ron / D * current * window_function * time_delta;
        w               = w_last + dw + direction * X_BORDER_BUMP;

        if (w >= D) {
            w = D;
        }
        if (w <= 0) {
            w = 0;
        }

        // calculate conductance
        G = 1 / (Ron * w / D + Roff * (1 - w / D));

        // set the current.
        V0.pot = vin;
        V0.flw = -vin / (rl * G + 1.0) * G;
        M0.pot = -vin / (rl * G + 1.0);
        M0.flw = -vin / (rl * G + 1.0) * G;
        RL.pot = -rl * vin / (rl * G + 1.0) * G;
        RL.flw = -vin / (rl * G + 1.0) * G;

        // persist variables
        w_last    = w;
        time_last = time;
    }
};
// =============================================================================

#if 0
`include "disciplines.vams"
`include "constants.vams"

`define X_BORDER_BUMP 10e-18

`define attr(txt) (*txt*)

module memristor1 (p,n);

    inout p,n;
    electrical p,n;

    parameter real Roff=16000 from (0:inf) `attr(info="Roff" type="instance");
    parameter real Ron=100 from (0:inf) `attr(info="Ron" type="instance");
    parameter real Rinit=11000 from (0:inf) `attr(info="Rinit" type="instance");
    parameter real D=10n from (0:inf) `attr(info="D" type="instance");
    parameter real uv=10e-15 from (0:inf) `attr(info="uv" type="instance");
    parameter real p_coeff=1.0 from (0:inf) `attr(info="p_coeff" type="instance");

    // local variables that should persist over time steps
    real w_last;
    real time_last;

    // local variables that hold temporary values
    real G;
    real window_function;
    real w;
    real dw;
    real R;
    real direction;
    real current;
    real time;
    real time_delta;

   
   analog
     begin

        @(initial_instance)
        begin
          w_last = ((Roff - Rinit) / (Roff - Ron)) * D;
          time_last = 0;
        end      

        // calculate conductance
        G = 1 / (Ron * w_last / D + Roff * (1 - w_last / D));
        current = G * V(p,n);


        direction = 0;
        if (current > 0) begin
          if(w_last<=0) begin
              direction=1;
          end
        end
        else begin
          if(w_last>=D) begin
                direction=-1;
          end
        end

        time = $realtime;
        time_delta = time - time_last;
        window_function = (1.0 - (pow(2 * w_last / D - 1, 2 * p_coeff)));
        dw = uv * Ron / D * current * window_function * time_delta;
        w = w_last + dw + direction * `X_BORDER_BUMP;

        if(w >= D) begin
          w = D;
        end
        if(w <= 0) begin
          w = 0;
        end

        // calculate conductance
        G = 1 / (Ron * w / D + Roff * (1 - w / D));

        // set the current
        I(p,n) <+ -1.0*G*V(p,n);

        // persist variables
        w_last = w;
        time_last = time;

     end
endmodule
#endif