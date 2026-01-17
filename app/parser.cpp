#include "parser.hpp"

#include <cctype>
#include <cstdlib>
#include <stdexcept>

Parser::Parser(std::string input) : input_(std::move(input)), pos_(0) {}

std::string Parser::parse_identifier() {
    skip_ws();
    if (pos_ >= input_.size() ||
        !(std::isalpha(input_[pos_]) || input_[pos_] == '_')) {
        throw std::runtime_error("expected identifier");
    }
    std::size_t start = pos_;
    while (pos_ < input_.size() &&
           (std::isalnum(input_[pos_]) || input_[pos_] == '_')) {
        ++pos_;
    }
    return input_.substr(start, pos_ - start);
}

double Parser::parse_number() {
    skip_ws();
    const char *start = input_.c_str() + pos_;
    char *end = nullptr;
    double value = std::strtod(start, &end);
    if (end == start) {
        throw std::runtime_error("expected number");
    }
    pos_ = static_cast<std::size_t>(end - input_.c_str());
    return value;
}

std::vector<double> Parser::parse_vector_literal() {
    expect('[', "expected '[' to start vector literal");
    skip_ws();
    std::vector<double> values;
    if (consume(']')) {
        return values;
    }
    values.push_back(parse_number());
    while (consume(',')) {
        values.push_back(parse_number());
    }
    expect(']', "expected ']' to end vector literal");
    return values;
}

std::vector<std::vector<double>> Parser::parse_matrix_literal() {
    expect('[', "expected '[' to start matrix literal");
    skip_ws();
    if (consume(']')) {
        throw std::runtime_error("matrix literal must contain rows");
    }
    std::vector<std::vector<double>> rows;
    rows.push_back(parse_vector_literal());
    if (rows.back().empty()) {
        throw std::runtime_error("matrix rows must not be empty");
    }
    while (consume(',')) {
        rows.push_back(parse_vector_literal());
        if (rows.back().empty()) {
            throw std::runtime_error("matrix rows must not be empty");
        }
    }
    expect(']', "expected ']' to end matrix literal");
    const std::size_t cols = rows.front().size();
    for (const auto &row : rows) {
        if (row.size() != cols) {
            throw std::runtime_error("all matrix rows must be same length");
        }
    }
    return rows;
}

bool Parser::consume(char c) {
    skip_ws();
    if (pos_ < input_.size() && input_[pos_] == c) {
        ++pos_;
        return true;
    }
    return false;
}

void Parser::expect(char c, const std::string &msg) {
    if (!consume(c)) {
        throw std::runtime_error(msg);
    }
}

void Parser::expect_end() {
    skip_ws();
    if (pos_ != input_.size()) {
        throw std::runtime_error("unexpected trailing input");
    }
}

bool Parser::empty() {
    skip_ws();
    return pos_ >= input_.size();
}

void Parser::skip_ws() {
    while (pos_ < input_.size() &&
           std::isspace(static_cast<unsigned char>(input_[pos_]))) {
        ++pos_;
    }
}
