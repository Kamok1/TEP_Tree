#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include "CTree.h"

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

bool parseCommand(const std::string& command, CTree& tree) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    bool state = true;
            
    if (cmd == "exit") {
        showMessage("Exiting program. Goodbye!");
        state = false;
    }

    else if (cmd == "enter") {
        std::string message;
        tree.buildTree(iss, message);
        if (!message.empty()) handleError(message);
    }

    else if (cmd == "join") {
        CTree otherTree;
        std::string message;
        otherTree.buildTree(iss, message);
        if(!message.empty()) handleError(message);
		else tree = tree + otherTree;
    }

    else if (cmd == "print") {
        std::vector<std::string> nodeValues;
        tree.getTreeNodeValues(nodeValues);
        printArray(nodeValues);
    }

    else if (cmd == "vars") {
        std::vector<std::string> vars;
        tree.getTreeVars(vars);
        printArray(vars);
    }

    else if (cmd == "comp") {
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
    return state;
}

int main() {
    CTree tree;
    bool running = true;
    std::string command;

    std::cout << "Enter commands (type 'exit' to quit):" << std::endl;
    while (running) {
        std::cout << "> ";
        std::getline(std::cin, command);
        running = parseCommand(command,tree);
    }

    return 0;
}