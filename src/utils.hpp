#ifndef __MINIC_UTILS_HPP__
#define __MINIC_UTILS_HPP__

#include <vector>
#include <optional>

#include "eeyore.hpp"

std::optional<int> getIdFromIndex(const std::vector<int> &widths,
                                  const std::vector<int> &index_list);

void removeUnnecessary(eeyore::FuncPtr ptr, std::vector<int> pos);


#endif // __MINIC_UTILS_HPP__
