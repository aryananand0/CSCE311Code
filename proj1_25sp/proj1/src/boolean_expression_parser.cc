// Copyright 2025 Aryan Anand
#include "boolean_expression_parser.h"

#include <cctype>
#include <iostream>

BooleanExpressionParser::BooleanExpressionParser(
    const std::string &expression,
    const std::unordered_map<char, bool> &values)
    : expression_(expression), values_(values), current_index_(0), error_(false) {}

void BooleanExpressionParser::skipWhitespace() {
  while (current_index_ < expression_.size() &&
         std::isspace(static_cast<unsigned char>(expression_[current_index_]))) {
    current_index_++;
  }
}

char BooleanExpressionParser::currentChar() {
  if (current_index_ < expression_.size()) {
    return expression_[current_index_];
  }
  return '\0';
}

void BooleanExpressionParser::consume() {
  if (current_index_ < expression_.size()) {
    current_index_++;
  }
}

bool BooleanExpressionParser::parse() {
  // Consume any leading whitespace.
  skipWhitespace();
  bool result = parseExpr();
  // Consume any trailing whitespace.
  skipWhitespace();
  if (current_index_ != expression_.size()) {
    reportError("Unexpected tokens after parsing. Remaining: '" +
                expression_.substr(current_index_) + "'");
    return false;
  }
  return result;
}

bool BooleanExpressionParser::hasError() const {
  return error_;
}

bool BooleanExpressionParser::parseExpr() {
  skipWhitespace();
  bool result = parseTerm();
  skipWhitespace();
  while (current_index_ < expression_.size() && currentChar() == '+') {
    consume();  // Consume the '+' operator.
    skipWhitespace();
    result = result || parseTerm();
    skipWhitespace();
  }
  return result;
}

bool BooleanExpressionParser::parseTerm() {
  skipWhitespace();
  bool result = parseFactor();
  skipWhitespace();
  while (current_index_ < expression_.size() && currentChar() == '*') {
    consume();  // Consume the '*' operator.
    skipWhitespace();
    result = result && parseFactor();
    skipWhitespace();
  }
  return result;
}

bool BooleanExpressionParser::parseFactor() {
  // Caller has already skipped whitespace.
  if (current_index_ >= expression_.size()) {
    reportError("Unexpected end of expression.");
    return false;
  }
  char token = currentChar();
  if (std::isalpha(static_cast<unsigned char>(token))) {
    consume();
    if (values_.find(token) != values_.end()) {
      return values_.at(token);
    } else {
      reportError("Undefined variable: " + std::string(1, token));
      return false;
    }
  } else {
    reportError("Unexpected token: " + std::string(1, token));
    return false;
  }
}

void BooleanExpressionParser::reportError(const std::string &message) {
  if (!error_) {
    std::cerr << "Error: " << message << std::endl;
    error_ = true;
  }
}
