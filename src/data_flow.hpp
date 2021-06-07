#ifndef __MINIC_DATA_FLOW_HPP__
#define __MINIC_DATA_FLOW_HPP__

#include <set>

#include "eeyore.hpp"

namespace data_flow {

typedef std::set<int> (*GenType)(eeyore::FunctionDef *, eeyore::InstPtr);

typedef void (*InitType)(eeyore::FunctionDef *, std::set<int> &);

typedef void (*MeetType)(std::set<int> &, std::set<int>);

void mUnion(std::set<int> &, std::set<int>);
void mIntersect(std::set<int> &, std::set<int>);
void mDifference(std::set<int> &, std::set<int>);

class Description {
public:
    GenType gen1_;
    GenType gen2_;
    InitType local_ident_;
    InitType block_init_;
    InitType global_init_;
    MeetType meet_;
};

} // namespace data_flow

#endif // __MINIC_DATA_FLOW_HPP__
