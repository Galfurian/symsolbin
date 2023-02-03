/// @file   model_gen.cpp
/// @author Enrico Fraccaroli
/// @date   Sep 20, 2021

#include "symsolbin/model/model_gen.hpp"

#include <cassert>

namespace symsolbin
{

static void __matrix_from_equations(
    const equation_set_t &eqns,
    const symbol_set_t &symbols,
    GiNaC::matrix &A,
    GiNaC::matrix &b)
{
    unsigned equ_size = static_cast<unsigned>(eqns.size());
    unsigned sym_size = static_cast<unsigned>(symbols.size());

    // build matrix from equation system
    GiNaC::matrix sys(equ_size, sym_size);
    GiNaC::matrix rhs(equ_size, 1);
    GiNaC::matrix vars(sym_size, 1);

    for (unsigned r = 0; r < equ_size; r++) {
        // lhs-rhs==0
        const GiNaC::ex eq = eqns[r].op(0) - eqns[r].op(1);
        GiNaC::ex linpart  = eq;
        for (unsigned c = 0; c < sym_size; c++) {
            const GiNaC::ex co = eq.coeff(GiNaC::ex_to<GiNaC::symbol>(symbols[c]), 1);
            linpart -= co * symbols[c];
            sys(r, c) = co;
        }
        linpart   = linpart.expand();
        rhs(r, 0) = -linpart;
    }
    A = sys;
    b = rhs;
}

std::string generate_class_dense(const analog_model_t &model, const std::string &name)
{
    std::stringstream ss;
    GiNaC::csrc_double(ss);
    
    auto structure = model.get_structure();
    auto solution = model.get_solution();
    auto system = model.get_system();

    GiNaC::matrix A;
    GiNaC::matrix b;

    __matrix_from_equations(system.equations, system.unknowns, A, b);

    unsigned equ_size = static_cast<unsigned>(system.equations.size());
    unsigned unk_size = static_cast<unsigned>(system.unknowns.size());

    ss << "//" << std::string(78, '=') << "\n";
    ss << "#include <Eigen/Dense>\n";
    ss << "#include <cassert>\n";
    ss << "\n";
    ss << "#include \"variableTracer.hpp\"\n";
    ss << "#include \"analog_pair.hpp\"\n";
    ss << "\n";
    ss << "class " << name << " {\n";
    ss << "public:\n";
    ss << "    Eigen::Matrix<double, " << A.rows() << ", " << A.cols() << "> A;\n";
    ss << "    Eigen::Matrix<double, " << b.rows() << ", 1> b;\n";
    ss << "\n";
    ss << "    Eigen::JacobiSVD<Eigen::Matrix<double, " << A.rows() << ", " << A.cols() << "> solver;\n";
    ss << "\n";
    for (const auto &edge : structure.edges)
        ss << "    analog_pair_t " << edge << ";\n";
    ss << "\n";
    ss << "    " << name << "() :\n";
    ss << "        A(),\n";
    ss << "        b(),\n";
    ss << "        solver(),\n";
    for (unsigned i = 0; i < structure.edges.size(); ++i) {
        ss << "        " << structure.edges[i] << "()";
        if (i < (structure.edges.size() - 1))
            ss << ",";
        ss << "\n";
    }
    ss << "    {\n";
#if 0
    ss << "        A << ";
    for (unsigned r = 0; r < equ_size; r++) {
        for (unsigned c = 0; c < unk_size; c++) {
            ss << A(r, c);
            if (c < (unk_size - 1))
                ss << ", ";
        }
        if (r < (equ_size - 1))
            ss << ",";
        else
            ss << ";";
        ss << "\n             ";
    }
#else
    ss << "        // Set the A matrix.\n";
    ss << "        A << ";
    for (unsigned r = 0; r < equ_size; r++) {
        for (unsigned c = 0; c < unk_size; c++) {
            ss << std::setw(4) << std::right << A(r, c);
            if (c < unk_size - 1)
                ss << ", ";
        }
        if (r < equ_size - 1)
            ss << ",\n             ";
        else
            ss << ";\n";
    }
    ss << "        // Set the b matrix.\n";
    ss << "        b << ";
    for (unsigned r = 0; r < equ_size; r++) {
        ss << std::setw(4) << std::right << b(r, 0);
        if (r < equ_size - 1)
            ss << ", ";
        else
            ss << ";\n";
    }
#endif
    ss << "\n";
    ss << "        solver.compute(A, Eigen::ComputeFullU | Eigen::ComputeFullV);\n";
    ss << "    }\n";
    ss << "\n";
    ss << "    void trace(VariableTracer & tracer)\n";
    ss << "    {\n";
    ss << "        tracer.addScope(\"" << name << "\");\n";
    for (const auto &edge : structure.edges)
        ss << "        tracer.addTrace(" << edge << ", \"" << edge.get_alias() << "\");\n";
    ss << "        tracer.closeScope();\n";
    ss << "    }\n";
    ss << "\n";
    ss << "    void run()\n";
    ss << "    {\n";
#if 0
    ss << "        b << ";
    for (unsigned r = 0; r < equ_size; r++) {
        //if (GiNaC::is_a<GiNaC::numeric>(b(r, 0))) {
        //    auto value = GiNaC::ex_to<GiNaC::numeric>(b(r, 0)).real().to_double();
        //    if (is_equal(value, 0.0))
        //        continue;
        //}
        ss << b(r, 0);
        if (r < (equ_size - 1))
            ss << ", ";
        else
            ss << ";\n";
    }
#else
#endif
    ss << "\n";
    ss << "        // Solve the matrix for the DIODE.\n";
    ss << "        auto solution = solver.solve(b);\n";
    ss << "\n";
    ss << "        // Get the solution.\n";
    for (unsigned r = 0; r < equ_size; r++) {
        ss << "        " << system.unknowns[r] << " = solution(" << r << ", 0);\n";
    }
    ss << "    }\n";
    ss << "};\n";
    ss << "//" << std::string(78, '=') << "\n";

    return ss.str();
}

std::string generate_class_sparse(const analog_model_t &model, const std::string &name)
{
    std::stringstream ss;
    GiNaC::csrc_double(ss);

    auto structure = model.get_structure();
    auto solution = model.get_solution();
    auto system = model.get_system();

    GiNaC::matrix A;
    GiNaC::matrix b;

    __matrix_from_equations(system.equations, system.unknowns, A, b);

    unsigned equ_size = static_cast<unsigned>(system.equations.size());
    unsigned unk_size = static_cast<unsigned>(system.unknowns.size());
    unsigned edg_size = static_cast<unsigned>(structure.edges.size());

    ss << "//" << std::string(78, '=') << "\n";
    ss << "#include <Eigen/Dense>\n";
    ss << "#include <cassert>\n";
    ss << "\n";
    ss << "#include \"variableTracer.hpp\"\n";
    ss << "#include \"analog_pair.hpp\"\n";
    ss << "\n";
    ss << "class " << name << " {\n";
    ss << "public:\n";
    ss << "    Eigen::Matrix<double, " << A.rows() << ", " << A.rows() << "> A;\n";
    ss << "    Eigen::Matrix<double, 1, " << A.rows() << "> b;\n";
    ss << "\n";
    ss << "    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;\n";
    ss << "\n";
    for (const auto &edge : structure.edges)
        ss << "    analog_pair_t " << edge << ";\n";
    ss << "\n";
    ss << "    " << name << "() :\n";
    ss << "        A(),\n";
    ss << "        b(),\n";
    ss << "        solver(),\n";
    for (unsigned i = 0; i < edg_size; ++i) {
        ss << "        " << structure.edges[i] << "()";
        if (i < (edg_size - 1))
            ss << ",";
        ss << "\n";
    }
    ss << "    {\n";
#if 0
    ss << "        A << ";
    for (unsigned r = 0; r < equ_size; r++) {
        for (unsigned c = 0; c < unk_size; c++) {
            ss << A(r, c);
            if (c < (unk_size - 1))
                ss << ", ";
        }
        if (r < (equ_size - 1))
            ss << ",";
        else
            ss << ";";
        ss << "\n             ";
    }
#else
    for (unsigned r = 0; r < equ_size; r++) {
        for (unsigned c = 0; c < unk_size; c++) {
            ss << "        A.insert(" << r << ", " << c << ") = " << A(r, c) << ";\n";
        }
    }
#endif
    ss << "\n";
    ss << "        solver.compute(A);\n";
    ss << "        assert((solver.info() == Eigen::Success) && \"Failed decomposing matrix.\");\n";
    ss << "    }\n";
    ss << "\n";
    ss << "    void trace(VariableTracer & tracer)\n";
    ss << "    {\n";
    ss << "        tracer.addScope(\"" << name << "\");\n";
    for (const auto &edge : structure.edges)
        ss << "        tracer.addTrace(" << edge << ", \"" << edge.get_alias() << "\");\n";
    ss << "        tracer.closeScope();\n";
    ss << "    }\n";
    ss << "\n";
    ss << "    void run()\n";
    ss << "    {\n";
    ss << "        // Set the b matrix.\n";
#if 0
    ss << "        b << ";
    for (unsigned r = 0; r < equ_size; r++) {
        //if (GiNaC::is_a<GiNaC::numeric>(b(r, 0))) {
        //    auto value = GiNaC::ex_to<GiNaC::numeric>(b(r, 0)).real().to_double();
        //    if (is_equal(value, 0.0))
        //        continue;
        //}
        ss << b(r, 0);
        if (r < (equ_size - 1))
            ss << ", ";
        else
            ss << ";\n";
    }
#else
    for (unsigned r = 0; r < equ_size; r++) {
        ss << "        b.insert(" << r << ", 0) = " << b(r, 0) << ";\n";
    }
#endif
    ss << "\n";

    ss << "        // Solve the matrix for the DIODE.\n";
    ss << "        auto solution = solver.solve(b);\n";
    ss << "        assert((solver.info() == Eigen::Success) && \"Failed solving the sparse matrix.\");\n";
    ss << "\n";
    ss << "        // Get the solution.\n";
    for (unsigned c = 0; c < unk_size; c++) {
        ss << "        " << system.unknowns[c] << " = solution(" << c << ", 0);\n";
    }
    ss << "    }\n";
    ss << "};\n";
    ss << "//" << std::string(78, '=') << "\n";

    return ss.str();
}

} // namespace symsolbin