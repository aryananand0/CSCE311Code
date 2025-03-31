#ifndef BOOL_EXPR_CLIENT_H
#define BOOL_EXPR_CLIENT_H

#include <string>
#include "domain_socket.h" // Provided library for UNIX domain socket operations

class BoolExprClient {
public:
    explicit BoolExprClient(const std::string &socketName);
    ~BoolExprClient();

    bool connectToServer();
    bool receiveSpecialChars(char &us, char &eot);
    bool sendTruthValues(const std::string &truthValues, char us, char eot);
    bool receiveSummary(std::string &summary);

private:
    std::string socketName_;
    DomainSocketClient client_; // Provided by teacher's library
    char eotChar_; // Stores the received EOT character for later use

    // Disable copy constructor and assignment.
    BoolExprClient(const BoolExprClient &) = delete;
    BoolExprClient &operator=(const BoolExprClient &) = delete;
};

#endif // BOOL_EXPR_CLIENT_H
