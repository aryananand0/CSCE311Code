# Boolean Expression Evaluation over UNIX Domain Sockets


## Purpose

This project implements a client-server system that uses UNIX domain sockets for interprocess communication (IPC). The **server** reads a file containing boolean expressions, builds a truth-value mapping for variables `a` through `z`, and evaluates each expression. The **client** connects to the server, receives two special characters (the unit separator and the end-of-transmission marker), sends truth value assignments (T/F) for the variables, and then receives a summary of the evaluation results.

---

## File Structure

The project directory (proj2/) contains only the following files:

proj2/
├── include/
│   ├── bool_expr_client.h
│   └── bool_expr_server.h
├── src/
│   ├── bool_expr_client.cc
│   └── bool_expr_server.cc
└── README.md


- **include/bool_expr_client.h**  
  Declares the interface for the client, which connects to the server, sends truth values, and receives the evaluation summary.

- **include/bool_expr_server.h**  
  Declares the interface for the server, which loads boolean expressions from a file, builds a truth mapping, evaluates the expressions, and sends back a summary.

- **src/bool_expr_client.cc**  
  Implements the client functionality. It connects to the server, receives the special characters (US and EOT), sends truth values (with unit separators), and prints the summary.

- **src/bool_expr_server.cc**  
  Implements the server functionality. It loads expressions (using std::getline), processes each expression (cleaned by Explode()), builds the mapping via BuildMap(), evaluates the expressions using a recursive descent parser, and sends back a summary in the format `<nTrue><US><nFalse><US><nError><EOT>`.

---

## Build Instructions

To build the project, open a terminal in the `proj2/` directory and run:

First Run "make", this command compiles the source files and produces two executables: bool-expr-client, bool-expr-server

Second start the server by running this `./bool-expr-server <expr_file> <socket_name> <us_char> <eot_char>`, Example: `./bool-expr-server dat/expr_25k.txt socket , .`

Third on a separate terminal run the client run this command `./bool-expr-client <socket_name> <T/F> [<T/F> ...]`, Example: `./bool-expr-client socket T T T T F F F F T T T T T T T`





