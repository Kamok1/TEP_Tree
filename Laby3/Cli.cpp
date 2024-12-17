#include "Cli.h"


CLI::CLI() {}

void CLI::run() {
    CTree tree;
    bool running = true;
    std::string command;

    showMessage(START_INFO_MESSAGE);
    while (running) {
        std::cout << CONSOLE_SIGN;
        std::getline(std::cin, command);
        parseCommand(command, tree, running);
    }
}

void CLI::parseCommand(const std::string& command, CTree& tree, bool& state) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == EXIT) {
        showMessage(EXIT_MESSAGE);
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
        CResult<double, CError> result = tree.compute(values, vars);
        if (result.bIsError()) {
            std::vector<CError*> errors = result.vGetErrors();
            handleCErrors(errors);
        }
        if (result.bIsSuccess()) showMessage(RESULT_MESSAGE + doubleToString(result.cGetValue()));
    }
    else handleError(UNKNOWN_COMMAND_MESSAGE);
}

void CLI::printArray(const std::vector<std::string>& array) {
    for (int i = 0; i < array.size(); i++)
    {
        std::cout << array[i] << " ";
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
        std::cerr << ERROR_MESSAGE << errors[i]->toString() << std::endl;
    }
}

std::string CLI::doubleToString(double value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
