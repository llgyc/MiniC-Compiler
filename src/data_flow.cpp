#include "data_flow.hpp"

namespace data_flow {

void mUnion(std::set<int> &s1, std::set<int> s2) {
    for (auto x : s2) {
        s1.insert(x);
    }
}

void mIntersect(std::set<int> &s1, std::set<int> s2) {
    std::set<int> ret;
    for (auto x : s1) {
        if (s2.find(x) != s2.end()) ret.insert(x);
    }
    s1 = ret;
}

void mDifference(std::set<int> &s1, std::set<int> s2) {
    std::set<int> ret;
    for (auto x : s1) {
        if (s2.find(x) == s2.end()) ret.insert(x);
    }
    s1 = ret;
}

} // namespace data_flow
