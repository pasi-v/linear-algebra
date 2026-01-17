#ifndef LA_CALC_PARSER_HPP
#define LA_CALC_PARSER_HPP

#include <cstddef>
#include <string>
#include <vector>

/** @brief Minimal parser for the REPL command language. */
class Parser {
  public:
    /**
     * @brief Construct a parser over a single input line.
     * @param input the line to parse.
     */
    explicit Parser(std::string input);

    /**
     * @brief Parse an identifier token ([A-Za-z_][A-Za-z0-9_]*).
     * @return the identifier.
     * @throws std::runtime_error if no identifier is present.
     */
    std::string parse_identifier();
    /**
     * @brief Parse a floating-point number.
     * @return the parsed value.
     * @throws std::runtime_error if no number is present.
     */
    double parse_number();

    /**
     * @brief Parse a vector literal like [1, 2, 3].
     * @return the parsed elements.
     * @throws std::runtime_error if the literal is malformed.
     */
    std::vector<double> parse_vector_literal();
    /**
     * @brief Parse a matrix literal like [[1,2],[3,4]].
     * @return the parsed rows.
     * @throws std::runtime_error if the literal is malformed.
     */
    std::vector<std::vector<double>> parse_matrix_literal();

    /**
     * @brief Consume a character if present, skipping leading whitespace.
     * @param c the character to consume.
     * @return true if the character was consumed, false otherwise.
     */
    bool consume(char c);
    /**
     * @brief Require a character, or throw with the provided message.
     * @param c the character to require.
     * @param msg the error message if the character is missing.
     * @throws std::runtime_error if the character is missing.
     */
    void expect(char c, const std::string &msg);
    /**
     * @brief Require end-of-input after skipping whitespace.
     * @throws std::runtime_error if non-whitespace input remains.
     */
    void expect_end();
    /**
     * @brief Check whether the remaining input is empty (after whitespace).
     * @return true if no input remains, false otherwise.
     */
    bool empty();

  private:
    /** @brief Advance past whitespace. */
    void skip_ws();

    std::string input_;
    std::size_t pos_;
};

#endif
