#include "evaluator.hpp"
#include "value.hpp"

#include <iostream>
#include <string>
#include <unordered_map>

#include <unistd.h>

int main() {
    std::unordered_map<std::string, Value> symbols;
    const bool interactive = isatty(fileno(stdin));

    for (std::string line; ; ) {
        if (interactive) {
            std::cout << "> " << std::flush;
        }
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (!execute_line(line, symbols, std::cout, std::cerr)) {
            break;
        }
    }

    return 0;
}
