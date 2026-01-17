#ifndef LA_CALC_PARSER_HPP
#define LA_CALC_PARSER_HPP

#include <cstddef>
#include <string>
#include <vector>

class Parser {
  public:
    explicit Parser(std::string input);

    std::string parse_identifier();
    double parse_number();

    std::vector<double> parse_vector_literal();
    std::vector<std::vector<double>> parse_matrix_literal();

    bool consume(char c);
    void expect(char c, const std::string &msg);
    void expect_end();
    bool empty();

  private:
    void skip_ws();

    std::string input_;
    std::size_t pos_;
};

#endif
