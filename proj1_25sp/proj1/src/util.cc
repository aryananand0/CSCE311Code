// Copyright 2025 Aryan Anand
#include "util.h"

#include <algorithm>
#include <cctype>
#include <vector>

namespace Util {

std::unordered_set<char> extractVariables(const std::string &expression) {
  std::unordered_set<char> vars;
  for (char c : expression) {
    if (std::isalpha(static_cast<unsigned char>(c))) {
      vars.insert(c);
    }
  }
  return vars;
}

bool validateValues(const std::unordered_set<char> &variables, int argc,
                    char *argv[]) {
  (void)argv;  // Mark argv as unused.
  int num_values = argc - 2;  // Values start at argv[2]
  return num_values == static_cast<int>(variables.size());
}

std::unordered_map<char, bool> mapValues(const std::unordered_set<char> &variables,
                                         int argc, char *argv[]) {
  (void)argc;  // Mark argc as unused.
  // Create a vector from the set and sort it in ascending order.
  std::vector<char> vars(variables.begin(), variables.end());
  std::sort(vars.begin(), vars.end());
  std::unordered_map<char, bool> mapping;
  int value_index = 2;  // Values start at argv[2]
  for (char var : vars) {
    // Use the first character of the argument: 'T' means true; otherwise false.
    mapping[var] = (argv[value_index][0] == 'T');
    value_index++;
  }
  return mapping;
}

}  // namespace Util
