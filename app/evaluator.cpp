#include "evaluator.hpp"

#include "la/matrix_algorithms.hpp"
#include "parser.hpp"
#include "la/vector_algorithms.hpp"

#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
la::Vector make_vector(const std::vector<double> &values) {
    la::Vector v(values.size());
    for (std::size_t i = 0; i < values.size(); ++i) {
        v[i] = values[i];
    }
    return v;
}

la::Matrix make_matrix(const std::vector<std::vector<double>> &rows) {
    if (rows.empty()) {
        throw std::runtime_error("matrix must have at least one row");
    }
    const std::size_t row_count = rows.size();
    const std::size_t col_count = rows.front().size();
    la::Matrix m(row_count, col_count);
    for (std::size_t r = 0; r < row_count; ++r) {
        for (std::size_t c = 0; c < col_count; ++c) {
            m(r, c) = rows[r][c];
        }
    }
    return m;
}

std::string trim(std::string s) {
    auto not_space = [](unsigned char ch) { return !std::isspace(ch); };
    while (!s.empty() && !not_space(static_cast<unsigned char>(s.front()))) {
        s.erase(s.begin());
    }
    while (!s.empty() && !not_space(static_cast<unsigned char>(s.back()))) {
        s.pop_back();
    }
    return s;
}

void print_help(std::ostream &out) {
    out << "Commands:\n"
        << "  vec <name> = [1, 2, 3]\n"
        << "  mat <name> = [[1,2],[3,4]]\n"
        << "  dot <vecA> <vecB>\n"
        << "  det <mat>\n"
        << "  rref <mat>\n"
        << "  in_span <b> <x1> <x2> ... <xn>\n"
        << "  print <name>\n"
        << "  help\n"
        << "  quit | exit\n";
}

void set_symbol(std::unordered_map<std::string, Value> &symbols,
                std::string name,
                Value value) {
    symbols.erase(name);
    symbols.emplace(std::move(name), std::move(value));
}

void handle_help(Parser &p, std::ostream &out) {
    p.expect_end();
    print_help(out);
}

void handle_vec(Parser &p, std::unordered_map<std::string, Value> &symbols) {
    std::string name = p.parse_identifier();
    p.expect('=', "expected '=' after vector name");
    auto values = p.parse_vector_literal();
    p.expect_end();
    Value v{Value::Kind::Vector, make_vector(values), la::Matrix(0, 0)};
    set_symbol(symbols, std::move(name), std::move(v));
}

void handle_mat(Parser &p, std::unordered_map<std::string, Value> &symbols) {
    std::string name = p.parse_identifier();
    p.expect('=', "expected '=' after matrix name");
    auto rows = p.parse_matrix_literal();
    p.expect_end();
    Value m{Value::Kind::Matrix, la::Vector(), make_matrix(rows)};
    set_symbol(symbols, std::move(name), std::move(m));
}

void handle_dot(Parser &p,
                std::unordered_map<std::string, Value> &symbols,
                std::ostream &out) {
    std::string a = p.parse_identifier();
    std::string b = p.parse_identifier();
    p.expect_end();
    if (!symbols.count(a) || !symbols.count(b)) {
        throw std::runtime_error("unknown symbol");
    }
    const Value &va = symbols.at(a);
    const Value &vb = symbols.at(b);
    if (va.kind != Value::Kind::Vector || vb.kind != Value::Kind::Vector) {
        throw std::runtime_error("dot expects two vectors");
    }
    out << dot(va.vec, vb.vec) << "\n";
}

void handle_det(Parser &p,
                std::unordered_map<std::string, Value> &symbols,
                std::ostream &out) {
    std::string name = p.parse_identifier();
    p.expect_end();
    if (!symbols.count(name)) {
        throw std::runtime_error("unknown symbol");
    }
    const Value &v = symbols.at(name);
    if (v.kind != Value::Kind::Matrix) {
        throw std::runtime_error("det expects a matrix");
    }
    out << la::determinant(v.mat) << "\n";
}

void handle_rref(Parser &p,
                 std::unordered_map<std::string, Value> &symbols,
                 std::ostream &out) {
    std::string name = p.parse_identifier();
    p.expect_end();
    if (!symbols.count(name)) {
        throw std::runtime_error("unknown symbol");
    }
    const Value &v = symbols.at(name);
    if (v.kind != Value::Kind::Matrix) {
        throw std::runtime_error("rref expects a matrix");
    }
    out << la::rref(v.mat);
}

void handle_print(Parser &p,
                  std::unordered_map<std::string, Value> &symbols,
                  std::ostream &out) {
    std::string name = p.parse_identifier();
    p.expect_end();
    if (!symbols.count(name)) {
        throw std::runtime_error("unknown symbol");
    }
    const Value &v = symbols.at(name);
    if (v.kind == Value::Kind::Vector) {
        out << v.vec << "\n";
    } else {
        out << v.mat;
    }
}

void handle_in_span(Parser &p,
                    std::unordered_map<std::string, Value> &symbols,
                    std::ostream &out) {
    // Parse the target vector b
    std::string b_name = p.parse_identifier();

    // Parse the spanning vectors x1, x2, ..., xn
    std::vector<std::string> spanning_names;
    while (!p.empty()) {
        spanning_names.push_back(p.parse_identifier());
    }

    if (spanning_names.empty()) {
        throw std::runtime_error("in_span expects at least one spanning vector");
    }

    // Verify b exists and is a vector
    if (!symbols.count(b_name)) {
        throw std::runtime_error("unknown symbol: " + b_name);
    }
    const Value &b_val = symbols.at(b_name);
    if (b_val.kind != Value::Kind::Vector) {
        throw std::runtime_error("in_span expects vectors, but " + b_name + " is not a vector");
    }

    // Collect spanning vectors and verify they exist and are vectors
    std::vector<la::Vector> spanning_vectors;
    for (const auto &name : spanning_names) {
        if (!symbols.count(name)) {
            throw std::runtime_error("unknown symbol: " + name);
        }
        const Value &v = symbols.at(name);
        if (v.kind != Value::Kind::Vector) {
            throw std::runtime_error("in_span expects vectors, but " + name + " is not a vector");
        }
        spanning_vectors.push_back(v.vec);
    }

    // Call is_in_span and output result
    la::Vector b = b_val.vec;
    bool result = la::is_in_span(spanning_vectors, b);
    out << (result ? "true" : "false") << "\n";
}
} // namespace

bool execute_line(const std::string &line,
                  std::unordered_map<std::string, Value> &symbols,
                  std::ostream &out,
                  std::ostream &err) {
    std::string trimmed = trim(line);
    if (trimmed.empty()) {
        return true;
    }
    if (!trimmed.empty() && trimmed[0] == '#') {
        return true;
    }

    try {
        Parser p(trimmed);
        std::string cmd = p.parse_identifier();

        if (cmd == "exit" || cmd == "quit") {
            p.expect_end();
            return false;
        }
        if (cmd == "help") {
            handle_help(p, out);
            return true;
        }
        if (cmd == "vec") {
            handle_vec(p, symbols);
            return true;
        }
        if (cmd == "mat") {
            handle_mat(p, symbols);
            return true;
        }
        if (cmd == "dot") {
            handle_dot(p, symbols, out);
            return true;
        }
        if (cmd == "det") {
            handle_det(p, symbols, out);
            return true;
        }
        if (cmd == "rref") {
            handle_rref(p, symbols, out);
            return true;
        }
        if (cmd == "print") {
            handle_print(p, symbols, out);
            return true;
        }
        if (cmd == "in_span") {
            handle_in_span(p, symbols, out);
            return true;
        }

        throw std::runtime_error("unknown command: " + cmd);
    } catch (const std::exception &e) {
        err << "error: " << e.what() << "\n";
        return true;
    }
}
