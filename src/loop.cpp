#include "loop.hpp"

bool inRange(int x, int l, int r) {
    return (x >= l) && (x <= r);
}

void calculateLoop(eeyore::FuncPtr func) {
    func->loop_pos_.clear();
    for (int i = 0; i < func->instNum(); ++i) {
        auto ptr = dynamic_cast<eeyore::CondInst *>(func->insts_[i].get());
        if (ptr != nullptr) {
            auto aim = func->label_pos_[ptr->label_id_];
            if (aim >= i) continue;
            bool flag = true;
            for (int j = aim; j < i; ++j) {
                auto ptr2 = dynamic_cast<eeyore::CondInst *>
                                (func->insts_[j].get());
                if (ptr2 != nullptr) {
                    if (!inRange(func->label_pos_[ptr2->label_id_], aim, i)) {
                        flag = false;
                        break;
                    }
                }
                auto ptr3 = dynamic_cast<eeyore::JumpInst *>
                                (func->insts_[j].get());
                if (ptr3 != nullptr) {
                    if (!inRange(func->label_pos_[ptr3->label_id_], aim, i)) {
                        flag = false;
                        break;
                    }
                }
            }
            if (flag) {
                func->loop_pos_.push_back(std::make_pair(aim, i));
            }
        }
    }
}
