#ifndef __MINIC_UTILS_HPP__
#define __MINIC_UTILS_HPP__

#include <vector>
#include <optional>

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

#endif // __MINIC_UTILS_HPP__
