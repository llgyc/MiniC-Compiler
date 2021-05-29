#ifndef __MINIC_OPERATOR_HPP__
#define __MINIC_OPERATOR_HPP__

#include <string>

enum class Operator {
#define EXPAND_OP(NAME, SYMBOL, INST) \
    NAME,
#include "operators.def"
#undef EXPAND_OP
};

const std::string operators[] = {
#define EXPAND_OP(NAME, SYMBOL, INST) \
    SYMBOL,
#include "operators.def"
#undef EXPAND_OP
};

const std::string instname[] = {
#define EXPAND_OP(NAME, SYMBOL, INST) \
    INST,
#include "operators.def"
#undef EXPAND_OP
};

#endif // __MINIC_OPERATOR_HPP__
