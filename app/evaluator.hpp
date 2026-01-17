#ifndef LA_CALC_EVALUATOR_HPP
#define LA_CALC_EVALUATOR_HPP

#include "value.hpp"

#include <ostream>
#include <string>
#include <unordered_map>

bool execute_line(const std::string &line,
                  std::unordered_map<std::string, Value> &symbols,
                  std::ostream &out,
                  std::ostream &err);

#endif
