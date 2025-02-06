// Copyright 2025 Aryan Anand
#include "boolean_expression_parser.h"
#include "util.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <expression> <values>\n";
    std::cerr << "Example: " << argv[0]
              << " \"a + b * c + d\" F T T T\n";
    return 1;
  }

  // Read the expression from the first argument.
  std::string expression = argv[1];

  // Extract the set of variables used in the expression.
  std::unordered_set<char> variables = Util::extractVariables(expression);

  // Validate that the number of provided values matches the number of variables.
  if (!Util::validateValues(variables, argc, argv)) {
    std::cerr << "Error: Number of values provided does not match the number "
                 "of variables in the expression.\n";
    return 1;
  }

  // Map the provided values to the corresponding variables.
  std::unordered_map<char, bool> values = Util::mapValues(variables, argc, argv);

  // Create the parser and evaluate the expression.
  BooleanExpressionParser parser(expression, values);
  bool result = parser.parse();

  if (parser.hasError()) {
    return 1;
  }

  std::cout << "Result of expression is: " << (result ? "True" : "False")
            << std::endl;
  return 0;
}
