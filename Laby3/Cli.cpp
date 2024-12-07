#include "cli.h"
#include <iostream>
#include <sstream>
#include <cmath>

CLI::CLI() {}

void CLI::run() {
    CTree tree;
    bool running = true;
    std::string command;

    showMessage("Enter commands (type '" EXIT "' to quit):");
    while (running) {
        std::cout << "> ";
        std::getline(std::cin, command);
        parseCommand(command, tree, running);
    }
}

void CLI::parseCommand(const std::string& command, CTree& tree, bool& state) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == EXIT) {
        showMessage("Exiting program. Goodbye!");
        state = false;
    }

    else if (cmd == ENTER) {
        CResult<void, CError> result = tree.buildTree(iss);
        if (result.bIsError()) handleCErrors(result.vGetErrors());
    }

    else if (cmd == JOIN) {
        CTree otherTree;
        CResult<void, CError> result = otherTree.buildTree(iss);
        if (result.bIsError()) handleCErrors(result.vGetErrors());
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
        double value;

        while (iss >> value) {
            values.push_back(value);
        }
        tree.getTreeVars(vars);

        if (values.size() != vars.size()) handleError("Mismatch between number of variables and provided values");
        else {
            CResult<double, CError> result = tree.compute(values, vars);
            if (result.bIsError()) handleCErrors(result.vGetErrors());
            else showMessage("Result:" + doubleToString(result.cGetValue()));
        }
    }
    else handleError("Unknown command");
}

void CLI::printArray(const std::vector<std::string>& array) {
    for (const auto& item : array) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void CLI::showMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void CLI::handleError(const std::string& error) {
    std::cerr << error << std::endl;
}

void CLI::handleCErrors(const std::vector<CError*>& errors) {
    for (int i = 0; i < errors.size(); i++) {
		std::cerr << errors[i]->toString() << std::endl;
	}
}

std::string CLI::doubleToString(double value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
