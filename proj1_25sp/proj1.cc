// Copyright CSCE 311 Spring 2024
//
// Code generated by ChatGPT in response to assignment prompt. Code submitted
// matching this form will not be accepted and, if too similar, will be sent
// to academic integrity as Falsification,
//   | Interfering with an instructor’s ability to evaluate accurately a
//   | student’s competency or performance on any academic work.
//
// The Generative AI has produced what is known as a
//   Top-down recursive descent parser
// To be fair, this is overkill for the assignment, but if interested, it is
// parsing a grammer based on the following grammar rules/description:
//   Expression -> Term { "+" Term }
//   Term -> Factor { "*" Factor }
//   Factor -> [a-z]
//

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class BooleanExpressionParser {
public:
    BooleanExpressionParser(const std::string& expression,
                            const std::unordered_map<char, bool>& values)
        : expression(expression), values(values), currentIndex(0), error(0) {
      // empty
    }

    bool parse() {
        bool result = parseExpr();
        if (error || currentIndex != expression.size()) {
            reportError("Unexpected tokens after parsing.");

            return false; // Indicate parse error
        }
        return result;
    }

    bool hasError() const {
        return error;
    }

private:
    std::string expression;
    const std::unordered_map<char, bool>& values;
    size_t currentIndex;
    bool error;

    // Get the current character
    char currentChar() const {
        if (currentIndex < expression.size()) {
            return expression[currentIndex];
        }
        return '\0';
    }

    // Advance to the next character
    void consume() {
        if (currentIndex < expression.size()) {
            currentIndex++;
        }
    }

    // Parse OR ('+') expressions
    bool parseExpr() {
        bool result = parseTerm(); // Parse AND expressions first
        while (currentChar() == '+') {
            consume(); // Consume the '+'
            result = result || parseTerm();
            if (error) return false; // Halt on error
        }
        return result;
    }

    // Parse AND ('x') expressions
    bool parseTerm() {
        bool result = parseFactor(); // Parse primary values first
        while (currentChar() == '*') {
            consume(); // Consume the '*'
            result = result && parseFactor();
            if (error) return false; // Halt on error
        }
        return result;
    }

    // Parse primary values: variables (a-z)
    bool parseFactor() {
        char token = currentChar();
        if (std::isalpha(token)) {
            consume();
            if (values.find(token) != values.end()) {
                return values.at(token);
            } else {
                reportError("Undefined variable: " + std::string(1, token));
                return false;
            }
        } else {
            reportError("Unexpected token: " + std::string(1, token));
            return false;
        }
    }

    void reportError(const std::string& message) {
        if (!error) {
            std::cerr << "Error: " << message << std::endl;
            error = true;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <expression> <values>\n";
        std::cerr << "Example: " << argv[0] << " \"a + b * c + a * d * e\" F T T T F\n";
        return 1;
    }

    // Read the expression
    std::string expression = argv[1];

    // Read variable values
    std::unordered_map<char, bool> values;
    size_t varIndex = 0;
    for (size_t i = 2; i < argc; ++i) {
        char var = 'a' + varIndex; // Variables are a, b, c, ...
        values[var] = (argv[i][0] == 'T');
        varIndex++;
    }

    // Create and use the parser
    BooleanExpressionParser parser(expression, values);
    bool result = parser.parse();

    if (parser.hasError()) {
        return 1;
    }

    // Output the result
    std::cout << "Result of expression is: " << (result ? "True" : "False") << std::endl;
    return 0;
}
