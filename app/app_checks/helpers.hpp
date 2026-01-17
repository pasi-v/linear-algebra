#ifndef LA_CALC_APP_CHECKS_HELPERS_HPP
#define LA_CALC_APP_CHECKS_HELPERS_HPP

#include "evaluator.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

struct EvalResult {
    bool keep_running;
    std::string out;
    std::string err;
};

inline EvalResult run_line(const std::string &line,
                           std::unordered_map<std::string, Value> &symbols) {
    std::ostringstream out;
    std::ostringstream err;
    bool keep_running = execute_line(line, symbols, out, err);
    return {keep_running, out.str(), err.str()};
}

#endif
