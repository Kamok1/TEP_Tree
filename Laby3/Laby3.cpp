#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include "CTree.h"
#define EXIT "exit"
#define ENTER "enter"
#define JOIN "join"
#define PRINT "print"
#define VARS "vars"
#define COMP "comp"

void printArray(const std::vector<std::string>& array) {
    for (int i = 0; i < array.size(); ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

void showMessage(const std::string& message) {
	std::cout << message << std::endl;
}

void handleError(const std::string& message) {
    std::cerr << message << std::endl;
}

std::string doubleToString(double value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void parseCommand(const std::string& command, CTree& tree, bool& state) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
            
    if (cmd == EXIT) {
        showMessage("Exiting program. Goodbye!");
        state = false;
    }

    else if (cmd == ENTER) {
        std::string message;
        tree.buildTree(iss, message);
        if (!message.empty()) handleError(message);
    }

    else if (cmd == JOIN) {
        CTree otherTree;
        std::string message;
        otherTree.buildTree(iss, message);
        if(!message.empty()) handleError(message);
		else tree = tree + otherTree;
    }

    else if (cmd == PRINT) {
        std::vector<std::string> nodeValues;
        tree.getTreeNodeValues(nodeValues);
        printArray(nodeValues);
    }

    else if (cmd == VARS) {
        std::vector<std::string> vars;
        tree.getTreeVars(vars);
        printArray(vars);
    }

    else if (cmd == COMP) {
        std::vector<double> values;
        std::vector<std::string> vars;
        std::string message;
        double value;
        double result;

        while (iss >> value) {
            values.push_back(value);
        }
        tree.getTreeVars(vars);

        if (values.size() != vars.size()) handleError("Mismatch between number of variables and provided values");
        else {
            tree.compute(result, values, vars, message);
            !message.empty() ? handleError(message) : showMessage("Result:" + doubleToString(result));
        }
    }
    else handleError("Unknown command");
}

int main() {
    CTree tree;
    bool running = true;
    std::string command;

    showMessage("Enter commands (type 'exit' to quit):");
    while (running) {
        std::cout << "> ";
        std::getline(std::cin, command);
        parseCommand(command,tree, running);
    }

    return 0;
}