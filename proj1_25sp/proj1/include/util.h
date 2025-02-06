// Copyright 2025 Aryan Anand
#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Util {

  // Extracts all unique alphabetical characters from the expression.
  std::unordered_set<char> extractVariables(const std::string &expression);

  // Validates that the number of provided values (argc - 2) equals the number of variables.
  bool validateValues(const std::unordered_set<char> &variables, int argc,
                      char *argv[]);

  // Maps the provided values to the variables.
  // The variables are sorted in ascending order, and the first provided value
  // corresponds to the smallest variable, the next to the next, etc.
  std::unordered_map<char, bool> mapValues(const std::unordered_set<char> &variables,
                                           int argc, char *argv[]);
}  // namespace Util

#endif  // UTIL_H_
