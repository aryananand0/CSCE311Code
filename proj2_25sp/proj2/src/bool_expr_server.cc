#include "bool_expr_server.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <unordered_map>



// Global variable for signal handling.
static volatile std::sig_atomic_t stopFlag = 0;
static std::string globalSocketName;

void handleSigInt(int signo) {
    if (signo == SIGINT) {
        stopFlag = 1;
        if (!globalSocketName.empty())
            ::remove(globalSocketName.c_str());
    }
}

BoolExprServer::BoolExprServer(const std::string &exprFile,
                               const std::string &socketName,
                               char us,
                               char eot)
    : exprFile_(exprFile), socketName_(socketName), usChar_(us), eotChar_(eot),
      server_(socketName.c_str(), us, eot, false) // false: use filesystem socket
{
    globalSocketName = socketName_;
}

BoolExprServer::~BoolExprServer() {
    ::remove(socketName_.c_str());
}

bool BoolExprServer::loadExpressions() {
    std::ifstream infile(exprFile_);
    if (!infile) {
        std::cerr << "Error: Unable to open expressions file: " << exprFile_ << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(infile, line)) {
        if (!line.empty())
            expressions_.push_back(line);
    }
    infile.close();
    if (expressions_.empty()) {
        std::cerr << "Error: No expressions loaded from file." << std::endl;
        return false;
    }
    return true;
}

int BoolExprServer::requiredVariables(const std::string &expression) {
    int req = 0;
    for (char ch : expression) {
        if (ch >= 'a' && ch <= 'z') {
            int varNum = ch - 'a' + 1;
            if (varNum > req)
                req = varNum;
        }
    }
    return req;
}

void BoolExprServer::processExpressions(const std::string &truthValues,
                                        int &countTrue,
                                        int &countFalse,
                                        int &countError) {
    std::string explodedTruth = Explode(truthValues.c_str(), ' ');
    auto mapping = BuildMap(explodedTruth);
    countTrue = countFalse = countError = 0;
    for (const auto &expr : expressions_) {
        std::string procExpr = Explode(expr.c_str(), ' ');
        int reqVars = requiredVariables(procExpr);
        if (reqVars > static_cast<int>(explodedTruth.size())) {
            countError++;
            continue;
        }
        BooleanExpressionParser parser(procExpr, mapping);
        bool result = parser.Parse();
        if (parser.HasError())
            countError++;
        else {
            if (result)
                countTrue++;
            else
                countFalse++;
        }
    }
}

void BoolExprServer::handleClient(int client_fd) {
    // Send special characters:
    // Build a message that consists of the US character and then append EOT manually.
    std::string specialMsg;
    specialMsg.push_back(usChar_);
    int s = server_.Write(client_fd, specialMsg);
    if ( s!= static_cast<ssize_t>(specialMsg.size()+1)) {
        std::cerr << "The server write size" << s << "The actual message size"<< specialMsg.size()<<std::endl;
        std::cerr << "Error: Failed to send special characters to client." << std::endl;
        return;
    }
    // Read the client's truth values message.
    std::string truthMsg;
    int received = server_.Read(client_fd, &truthMsg);
    if ( received <= 0) {
        std::cerr << "Error: Failed to receive truth values from client." << std::endl;
        return;
    }
    std::cerr << "           "<< s <<"B sent, "<< received << "B received" << std::endl;
    
    
    std::cerr << std::dec << std::endl;
    
    // Look for the EOT character.
    size_t pos = truthMsg.find(eotChar_);
    if (pos == std::string::npos) {
        std::cerr << "Error: End-of-transmission character not found in client's message." << std::endl;
        return;
    }
    std::string truthValues = truthMsg.substr(0, pos);
    // Remove any unit separator characters.
    truthValues.erase(std::remove(truthValues.begin(), truthValues.end(), usChar_), truthValues.end());
    
    std::cout << "Received truth values: " << truthValues << std::endl;
    
    int countTrue, countFalse, countError;
    processExpressions(truthValues, countTrue, countFalse, countError);
    
    // Format summary: <countTrue><US><countFalse><US><countError>
    std::string summary = std::to_string(countTrue) + usChar_ +
                          std::to_string(countFalse) + usChar_ +
                          std::to_string(countError);
    // Manually append EOT.
    summary.push_back(eotChar_);
    if (server_.Write(client_fd, summary) != static_cast<ssize_t>(summary.size()+1)) {
        std::cerr << "Error: Failed to send summary to client." << std::endl;
    }
    std::cout << "Sent summary: " << summary << std::endl;
    std::cerr << "Client disconnected" << std::endl;

}

bool BoolExprServer::start() {
    if (!loadExpressions())
        return false;
    ::remove(socketName_.c_str());
    if (!server_.Init(5)) { // Allow up to 5 pending connections.
        std::cerr << "Error: Failed to initialize server socket." << std::endl;
        return false;
    }
    return true;
}

void BoolExprServer::run() {
    std::signal(SIGINT, handleSigInt);
    std::cout << "BoolExprServer running." << std::endl;
    std::cout << "Expressions file: " << exprFile_ << std::endl;
    std::cout << "Listening on socket: " << socketName_ << std::endl;
    while (!stopFlag) {
        int client_fd = server_.Accept();
        if (client_fd < 0) {
            if (errno == EINTR && stopFlag)
                break;
            std::cerr << "Warning: Accept failed: " << std::strerror(errno) << std::endl;
            continue;
        }
        std::cerr << "Client connected" << std::endl;
        handleClient(client_fd);
        close(client_fd);
    }
    std::cout << "Server shutting down." << std::endl;
}

// Main function for the server application
int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0]
                  << " <expr_file> <socket_name> <us_char> <eot_char>" << std::endl;
        return 1;
    }
    std::string exprFile = argv[1];
    std::string socketName = argv[2];
    std::string us_String = argv[3];
    char us = argv[3][0];
    char eot = argv[4][0];
    std::cerr << "This is us" << us << "This is eot"<<eot<<std::endl;

    BoolExprServer server(exprFile, socketName, us, eot);
    if (!server.start())
        return 1;
    server.run();
    return 0;
}
