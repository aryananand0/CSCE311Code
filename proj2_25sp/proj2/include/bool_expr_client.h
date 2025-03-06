#ifndef BOOL_EXPR_CLIENT_H
#define BOOL_EXPR_CLIENT_H

#include <string>
#include "domain_socket.h" // Provided library for UNIX domain socket operations

/**
 * @class BoolExprClient
 * @brief Client for sending truth value assignments to the boolean expression evaluation server.
 *
 * The client connects to the server via a UNIX domain socket, receives two special characters
 * (unit separator and end-of-transmission), sends a sequence of truth values (T/F) separated by the unit
 * separator and terminated by the end-of-transmission character, then receives a summary response.
 */
class BoolExprClient {
public:
    /**
     * @brief Constructs a BoolExprClient for the given socket.
     * @param socketName Name or path of the server's UNIX domain socket.
     */
    explicit BoolExprClient(const std::string &socketName);

    /**
     * @brief Destructor.
     */
    ~BoolExprClient();

    /**
     * @brief Connects to the server.
     * @return true if connection succeeds, false otherwise.
     */
    bool connectToServer();

    /**
     * @brief Receives the two special characters from the server.
     * @param us Output parameter: the unit separator.
     * @param eot Output parameter: the end-of-transmission character.
     * @return true if both characters are received, false otherwise.
     */
    bool receiveSpecialChars(char &us, char &eot);

    /**
     * @brief Sends the truth values to the server.
     * @param truthValues A string of truth values (e.g., "TFTFT...").
     * @param us The unit separator character to insert between values.
     * @param eot The termination (end-of-transmission) character.
     * @return true if sending succeeds, false otherwise.
     */
    bool sendTruthValues(const std::string &truthValues, char us, char eot);

    /**
     * @brief Receives the summary response from the server.
     * @param summary Output parameter to store the received summary.
     * @return true if successful, false otherwise.
     */
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
