#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>
#include "CTree.h"
#include "CResultVoid.h"

#define EXIT "exit"
#define ENTER "enter"
#define JOIN "join"
#define PRINT "print"
#define VARS "vars"
#define COMP "comp"

class CLI {
public:
    CLI();
    void run();

private:
    void parseCommand(const std::string& command, CTree& tree, bool& state);
    void printArray(const std::vector<std::string>& array);
    void showMessage(const std::string& message);
    void handleError(const std::string& message);
    void handleCErrors(const std::vector<CError*>& errors);
    std::string doubleToString(double value);
};

#endif
