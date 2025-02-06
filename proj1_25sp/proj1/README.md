<!-- Your info goes here -->
## File Overviews

- **`boolean_expression_parser.h/cc`**  
  This file defines and implements the `BooleanExpressionParser` class, which parses Boolean expressions using:
  - **`'+'`** for logical OR
  - **`'*'`** for logical AND  
  It automatically handles extra whitespace and ensures that no unparsed tokens remain.

- **`util.h/cc`**  
  These files provide helper functions within the `Util` namespace:
  - **`extractVariables`**: Finds all unique alphabetical characters in the expression and returns them as a set.
  - **`validateValues`**: Verifies that the number of Boolean values matches the number of variables in the expression.
  - **`mapValues`**: Maps the provided Boolean values to the correct variables, sorting them alphabetically.

- **`main.cc`**  
  This file contains the `main` function, which:
  1. Reads the Boolean expression and corresponding values from the command line.
  2. Uses the helper functions to extract variables and map their values.
  3. Instantiates the parser and prints the final evaluation result.

# Project Directory Structure
proj1/
├── Makefile                    # Build configuration file for make program
|
├── src/
│   ├── boolean_expression_parser.cc  # Source file for the parser logic
│   ├── main.cc                       # Main entry point to program
│   ├── util.cc                       # Source file for utility functions
|
├── include/
│   ├── boolean_expression_parser.h   # Header file for the parser logic
│   ├── util.h                        # Header file for utility declarations
|
├── bin/                        # Build directory (generated during build process)
│   ├── evaluate-boolean              # Main entry point of the program
|
├── build/                      # Compile directory (generated during build process)
│   ├── boolean_expression_parser.o   # object for the parser logic
│   ├── main.o                        # object for the+ main entry point to program
│   ├── util.o                        # object for utility functions
|
└── README.md                   # This file

