#ifndef LA_CALC_EVALUATOR_HPP
#define LA_CALC_EVALUATOR_HPP

#include "value.hpp"

#include <ostream>
#include <string>
#include <unordered_map>

/**
 * @brief Execute a single REPL line.
 * @param line the input line to execute.
 * @param symbols the mutable symbol table.
 * @param out output stream for results.
 * @param err error stream for diagnostics.
 * @return false to signal exit, true to continue.
 */
bool execute_line(const std::string &line,
                  std::unordered_map<std::string, Value> &symbols,
                  std::ostream &out,
                  std::ostream &err);

#endif
