#ifndef __MINIC_LIVENESS_HPP__
#define __MINIC_LIVENESS_HPP__

#include "eeyore.hpp"
#include "data_flow.hpp"

namespace liveness {

data_flow::GenType def;
data_flow::GenType use;

void setG(int);
void setN(int);
void setT(int);
void setP(int);

void calculate(eeyore::Program &ir);
void optimize(eeyore::Program &ir, int times = 1);

} // namespace liveness

#endif // __MINIC_LIVENESS_HPP__
