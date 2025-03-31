#ifndef BOOL_EXPR_SERVER_H
#define BOOL_EXPR_SERVER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "domain_socket.h"    
#include "bool_expr_parser.h" // Provided boolean expression parser


class BoolExprServer {
public:
    
    BoolExprServer(const std::string &exprFile,
                   const std::string &socketName,
                   char us,
                   char eot);

    
    ~BoolExprServer();

    
    bool start();

    /**
     * @brief Runs the server: accepts client connections and processes requests.
     */
    void run();

private:
    std::string exprFile_;
    std::vector<std::string> expressions_;
    std::string socketName_;
    char usChar_;
    char eotChar_;
    DomainSocketServer server_; // Teacher's DomainSocketServer

    /**
     * @brief Loads expressions from the file.
     * @return true if expressions are loaded successfully, false otherwise.
     */
    bool loadExpressions();

    /**
     * @brief Determines the number of variables required by an expression.
     * @param expression The boolean expression.
     * @return The number of distinct variables required.
     */
    int requiredVariables(const std::string &expression);

    /**
     * @brief Evaluates all loaded expressions using the provided truth values mapping.
     * @param truthValues The truth values string received from the client.
     * @param countTrue Output count of expressions evaluating to true.
     * @param countFalse Output count of expressions evaluating to false.
     * @param countError Output count of expressions that could not be evaluated.
     */
    void processExpressions(const std::string &truthValues,
                            int &countTrue,
                            int &countFalse,
                            int &countError);

    /**
     * @brief Handles an individual client connection.
     * @param client_fd File descriptor for the connected client.
     */
    void handleClient(int client_fd);

    // Disable copy constructor and assignment.
    BoolExprServer(const BoolExprServer &) = delete;
    BoolExprServer &operator=(const BoolExprServer &) = delete;
};

#endif // BOOL_EXPR_SERVER_H
