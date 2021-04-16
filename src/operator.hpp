#ifndef __MINIC_OPERATOR_HPP__
#define __MINIC_OPERATOR_HPP__

enum class Operator {
#define EXPAND_OP(NAME, SYMBOL) \
    NAME,
#include "operators.def"
#undef EXPAND_OP
};

#endif // __MINIC_OPERATOR_HPP__
