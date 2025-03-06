#include "bool_expr_client.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>  // for std::strerror
#include <unistd.h>


BoolExprClient::BoolExprClient(const std::string &socketName)
    : socketName_(socketName), client_(socketName.c_str(), false), eotChar_('\0') {}

BoolExprClient::~BoolExprClient() {
    // DomainSocketClient destructor will clean up the socket.
}

bool BoolExprClient::connectToServer() {
    if (!client_.Init()) {
        std::cerr << "Error: Unable to connect to server at " << socketName_
                  << ": " << std::strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool BoolExprClient::receiveSpecialChars(char &us, char &eot) {
    std::string buf1, buf2;
    // Read one byte for US.
    int received = client_.Read(static_cast<size_t>(1), &buf1);
    if( received!= 1) {
        std::cerr << "Error: Failed to read US character from server." << std::endl;
        return false;
    }
    // Read one byte for EOT.
    if (client_.Read(static_cast<size_t>(1), &buf2) != 1) {
        std::cerr << "Error: Failed to read EOT character from server." << std::endl;
        return false;
    }
    us = buf1[0];
    eot = buf2[0];
    eotChar_ = eot; // Store the EOT character for later use.
    std::cerr << "Received "<<received<<"B";
    return true;
}

bool BoolExprClient::sendTruthValues(const std::string &truthValues, char us, char eot) {
    // Build message: truth values separated by US.
    std::string message;
    for (size_t i = 0; i < truthValues.size(); ++i) {
        message.push_back(truthValues[i]);
        if (i != truthValues.size() - 1)
            message.push_back(us);
    }
        message.push_back(eot);
    
    
    std::cerr << std::dec << std::endl;
    
    // Call Write() with the message; now the message already contains the EOT.
    ::ssize_t sent = client_.Write(message, eot);
    // Since our message already contains the EOT, we expect exactly message.size() bytes.
    if (sent != static_cast<ssize_t>(message.size()+1)) {
        std::cerr << "Error: Failed to send truth values to server. Expected " 
                  << message.size() << " bytes, but sent " << sent 
                  << " bytes. errno: " << std::strerror(errno) << std::endl;
        return false;
    }
    std::cerr <<"Sent "<< sent<<"B"<<std::endl;
    return true;
}

bool BoolExprClient::receiveSummary(std::string &summary) {
    // Use the Read overload that reads until the EOT character is encountered.
    if (client_.Read(eotChar_, &summary) <= 0) {
        std::cerr << "Error: Failed to receive summary from server." << std::endl;
        return false;
    }
    // Debug: print the received summary in hex.
    
    std::cerr << std::dec << std::endl;
    return true;
}

int main(int argc, char *argv[]) {
    // Expect at least: program name, socket name, and one truth value.
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <socket_name> <T/F> [<T/F> ...]" << std::endl;
        return 1;
    }
    std::string socketName = argv[1];
    std::string truthValues;
    for (int i = 2; i < argc; ++i) {
        std::string val(argv[i]);
        if (val != "T" && val != "F") {
            std::cerr << "Error: Invalid truth value \"" << val << "\". Use T or F." << std::endl;
            return 1;
        }
        truthValues.push_back(val[0]);
    }
    std::cout << "BoolExprClient connecting..." << std::endl;
    BoolExprClient client(socketName);
    if (!client.connectToServer())
        return 1;
    char us, eot;
    if (!client.receiveSpecialChars(us, eot))
        return 1;
    if (!client.sendTruthValues(truthValues, us, eot))
        return 1;
    std::string summary;
    if (!client.receiveSummary(summary))
        return 1;
    // Parse summary: expected format is <nTrue><US><nFalse><US><nError>
    std::istringstream iss(summary);
    std::string part;
    std::vector<std::string> parts;
    while (std::getline(iss, part, us)) {
        parts.push_back(part);
    }
    if (parts.size() != 3) {
        std::cerr << "Error: Unexpected summary format received." << std::endl;
        return 1;
    }
    std::cout << "Results" << std::endl;
    std::cout << "True Evaluations: " << parts[0] << std::endl;
    std::cout << "False Evaluations: " << parts[1] << std::endl;
    std::cout << "Could Not Evaluate: " << parts[2] << std::endl;
    return 0;
}
