#include "evaluator.hpp"

#include "la/matrix_algorithms.hpp"
#include "la/matrix_linear_systems.hpp"
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
        << "  in_span <b> <A>\n"
        << "  lin_indep <v1> <v2> ... <vn>\n"
        << "  lin_indep <M1> <M2> ... <Mn>\n"
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
        throw std::runtime_error("in_span expects at least one spanning vector or a matrix");
    }

    // Verify b exists and is a vector
    if (!symbols.count(b_name)) {
        throw std::runtime_error("unknown symbol: " + b_name);
    }
    const Value &b_val = symbols.at(b_name);
    if (b_val.kind != Value::Kind::Vector) {
        throw std::runtime_error("in_span: " + b_name + " must be a vector");
    }

    // Dispatch on the type of the first spanning argument
    const std::string &first = spanning_names.front();
    if (!symbols.count(first)) {
        throw std::runtime_error("unknown symbol: " + first);
    }

    if (symbols.at(first).kind == Value::Kind::Matrix) {
        // Matrix form: in_span <b> <A>
        if (spanning_names.size() > 1) {
            throw std::runtime_error("in_span: cannot mix matrix and vector arguments");
        }
        bool result = la::is_in_span(symbols.at(first).mat, b_val.vec);
        out << (result ? "true" : "false") << "\n";
        return;
    }

    // Vector form: in_span <b> <v1> <v2> ... <vn>
    std::vector<la::Vector> spanning_vectors;
    for (const auto &name : spanning_names) {
        if (!symbols.count(name)) {
            throw std::runtime_error("unknown symbol: " + name);
        }
        const Value &v = symbols.at(name);
        if (v.kind != Value::Kind::Vector) {
            throw std::runtime_error("in_span: cannot mix matrix and vector arguments");
        }
        spanning_vectors.push_back(v.vec);
    }

    bool result = la::is_in_span(spanning_vectors, b_val.vec);
    out << (result ? "true" : "false") << "\n";
}

void handle_lin_indep(Parser &p,
                      std::unordered_map<std::string, Value> &symbols,
                      std::ostream &out) {
    // Collect all argument names, then dispatch on the kind of the first.
    std::vector<std::string> names;
    while (!p.empty()) {
        names.push_back(p.parse_identifier());
    }

    if (names.empty()) {
        throw std::runtime_error(
            "lin_indep expects at least one vector or matrix");
    }

    const std::string &first = names.front();
    if (!symbols.count(first)) {
        throw std::runtime_error("unknown symbol: " + first);
    }

    if (symbols.at(first).kind == Value::Kind::Matrix) {
        // Matrix form: lin_indep <M1> <M2> ... <Mn>
        std::vector<la::Matrix> matrices;
        for (const auto &name : names) {
            if (!symbols.count(name)) {
                throw std::runtime_error("unknown symbol: " + name);
            }
            const Value &v = symbols.at(name);
            if (v.kind != Value::Kind::Matrix) {
                throw std::runtime_error(
                    "lin_indep: cannot mix matrix and vector arguments");
            }
            matrices.push_back(v.mat);
        }
        bool result = la::are_linearly_independent(matrices);
        out << (result ? "true" : "false") << "\n";
        return;
    }

    // Vector form: lin_indep <v1> <v2> ... <vn>
    std::vector<la::Vector> vectors;
    for (const auto &name : names) {
        if (!symbols.count(name)) {
            throw std::runtime_error("unknown symbol: " + name);
        }
        const Value &v = symbols.at(name);
        if (v.kind != Value::Kind::Vector) {
            throw std::runtime_error("lin_indep: " + name + " must be a vector");
        }
        vectors.push_back(v.vec);
    }

    bool result = la::are_linearly_independent(vectors);
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
        if (cmd == "lin_indep") {
            handle_lin_indep(p, symbols, out);
            return true;
        }

        throw std::runtime_error("unknown command: " + cmd);
    } catch (const std::exception &e) {
        err << "error: " << e.what() << "\n";
        return true;
    }
}
