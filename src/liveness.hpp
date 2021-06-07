#ifndef __MINIC_LIVENESS_HPP__
#define __MINIC_LIVENESS_HPP__

#include "eeyore.hpp"
#include "data_flow.hpp"

namespace liveness {

void calculate(eeyore::Program &ir);
void optimize(eeyore::Program &ir);

} // namespace liveness

#endif // __MINIC_LIVENESS_HPP__
