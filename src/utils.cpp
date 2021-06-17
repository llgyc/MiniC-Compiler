#include "utils.hpp"

std::optional<int> getIdFromIndex(const std::vector<int> &widths,
                                  const std::vector<int> &index_list) {
    if (widths.size() != index_list.size())
        return std::nullopt;
    int ret = 0;
    for (unsigned i = 0; i + 1 < index_list.size(); ++i)
        ret += index_list[i] * widths[i+1];
    ret += index_list.back();
    return ret;
}

void removeUnnecessary(eeyore::FuncPtr ptr, std::vector<int> pos, 
    bool radical) {
    eeyore::InstPtrList ret;
    std::vector<int> npos;
    unsigned p = 0;
    for (int i = 0; i < ptr->instNum(); ++i) {
        if (p < pos.size() && i == pos[p]) {
            if (!radical) {
                auto tmp = dynamic_cast<eeyore::AssignCallInst *>
                    (ptr->insts_[i].get());
                if (tmp != nullptr) {
                    auto inst = std::make_shared<eeyore::AssignCallInst>
                        (tmp->func_);
                    npos.push_back(i);
                    ret.push_back(inst);
                }
            }
            ++p; continue;
        }
        npos.push_back(i);
        ret.push_back(ptr->insts_[i]);
    }
    ptr->insts_ = ret;
    for (auto &x : ptr->label_pos_) {
        auto nx = std::lower_bound(npos.begin(), npos.end(), x) - npos.begin();
        x = nx;
    }
}
