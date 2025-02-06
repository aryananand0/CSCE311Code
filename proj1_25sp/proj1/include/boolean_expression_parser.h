// Copyright 2025 Aryan Anand
#ifndef BOOLEAN_EXPRESSION_PARSER_H_
#define BOOLEAN_EXPRESSION_PARSER_H_

#include <string>
#include <unordered_map>

class BooleanExpressionParser {
 public:
  // Constructs a parser with the given expression and variable values.
  BooleanExpressionParser(const std::string &expression,
                          const std::unordered_map<char, bool> &values);

  // Parses the expression and returns its Boolean value.
  // If an error is encountered, an error message is printed and false is returned.
  bool parse();

  // Returns true if an error was encountered during parsing.
  bool hasError() const;

 private:
  std::string expression_;
  const std::unordered_map<char, bool> &values_;
  size_t current_index_;
  bool error_;

  // Returns the character at the current index (without skipping whitespace).
  char currentChar();

  // Advances the current index by one.
  void consume();

  // Advances over any whitespace characters.
  void skipWhitespace();

  // Recursive descent parser functions.
  bool parseExpr();
  bool parseTerm();
  bool parseFactor();

  // Reports an error with the given message (only once).
  void reportError(const std::string &message);
};

#endif  // BOOLEAN_EXPRESSION_PARSER_H_
