/// @file   equationClassifier.hpp
/// @author Enrico Fraccaroli
/// @date   Sep 20, 2021

#include "symsolbin/solver/classifier.hpp"

namespace symsolbin
{

classifier_t::classifier_t()
{
}

classifier_t::~classifier_t()
{
}

bool classifier_t::visit(const GiNaC::ex &e)
{
    if (GiNaC::is_a<GiNaC::relational>(e)) {
        return this->visitRelational(GiNaC::ex_to<GiNaC::relational>(e));
    } else if (GiNaC::is_a<GiNaC::symbol>(e)) {
        return this->visitSymbol(GiNaC::ex_to<GiNaC::symbol>(e));
    } else if (GiNaC::is_a<GiNaC::constant>(e)) {
        return this->visitConstant(GiNaC::ex_to<GiNaC::constant>(e));
    } else if (GiNaC::is_a<GiNaC::add>(e)) {
        return this->visitAdd(e);
    } else if (GiNaC::is_a<GiNaC::mul>(e)) {
        return this->visitMul(e);
    } else if (GiNaC::is_a<GiNaC::numeric>(e)) {
        return this->visitNumeric(GiNaC::ex_to<GiNaC::numeric>(e));
    } else if (GiNaC::is_a<GiNaC::function>(e)) {
        return this->visitFunction(GiNaC::ex_to<GiNaC::function>(e));
    } else if (GiNaC::is_a<GiNaC::power>(e)) {
        return this->visitPower(GiNaC::ex_to<GiNaC::power>(e));
    }
    std::cout << "classifier_t: GiNaC expression not handled `" << e << "`...";
    return false;
}

bool classifier_t::visitRelational(const GiNaC::relational &e)
{
    return this->visit(e.lhs()) && this->visit(e.rhs());
}

bool classifier_t::visitSymbol(const GiNaC::symbol &)
{
    return true;
}

bool classifier_t::visitConstant(const GiNaC::constant &)
{
    return true;
}

bool classifier_t::visitAdd(const GiNaC::ex &e)
{
    for (const GiNaC::ex &value : e)
        if (!this->visit(value))
            return false;
    return true;
}

bool classifier_t::visitMul(const GiNaC::ex &e)
{
    for (const GiNaC::ex &value : e)
        if (!this->visit(value))
            return false;
    return true;
}

bool classifier_t::visitNumeric(const GiNaC::numeric &)
{
    return true;
}

bool classifier_t::visitFunction(const GiNaC::function &)
{
    return false;
}

bool classifier_t::visitPower(const GiNaC::power &e)
{
    for (size_t i = 0; i < e.nops(); ++i)
        if (!this->visit(e.op(i)))
            return false;
    return true;
}

} // namespace symsolbin