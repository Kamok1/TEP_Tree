//#include <iostream>
//#include <vector>
//#include <string>
//#include <sstream>
//#include <cmath>
//
//class CNode {
//public:
//    std::string value;
//    std::vector<CNode*> children;
//
//    CNode(const std::string& val) : value(val) {}
//
//    ~CNode() {
//        for (size_t i = 0; i < children.size(); ++i) {
//            delete children[i];
//        }
//    }
//};
//
//class CTree {
//private:
//    CNode* root;
//
//    void deleteTree(CNode* node) {
//        if (node) {
//            for (size_t i = 0; i < node->children.size(); ++i) {
//                deleteTree(node->children[i]);
//            }
//            delete node;
//        }
//    }
//
//    void printNode(CNode* node) const {
//        if (!node) return;
//        std::cout << node->value << " ";
//        for (size_t i = 0; i < node->children.size(); ++i) {
//            printNode(node->children[i]);
//        }
//    }
//
//    void collectVars(CNode* node, std::vector<std::string>& vars) const {
//        if (!node) return;
//        if (isVariable(node->value)) {
//            if (std::find(vars.begin(), vars.end(), node->value) == vars.end()) {
//                vars.push_back(node->value);
//            }
//        }
//        for (size_t i = 0; i < node->children.size(); ++i) {
//            collectVars(node->children[i], vars);
//        }
//    }
//
//    double evaluateNode(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values) const {
//        if (!node) throw std::runtime_error("Invalid node");
//
//        if (isOperator(node->value)) {
//            if (node->value == "+") return evaluateNode(node->children[0], vars, values) + evaluateNode(node->children[1], vars, values);
//            if (node->value == "-") return evaluateNode(node->children[0], vars, values) - evaluateNode(node->children[1], vars, values);
//            if (node->value == "*") return evaluateNode(node->children[0], vars, values) * evaluateNode(node->children[1], vars, values);
//            if (node->value == "/") return evaluateNode(node->children[0], vars, values) / evaluateNode(node->children[1], vars, values);
//            if (node->value == "sin") return std::sin(evaluateNode(node->children[0], vars, values));
//            if (node->value == "cos") return std::cos(evaluateNode(node->children[0], vars, values));
//        }
//        else if (isVariable(node->value)) {
//            for (size_t i = 0; i < vars.size(); ++i) {
//                if (vars[i] == node->value) {
//                    return values[i];
//                }
//            }
//            throw std::runtime_error("Variable not provided: " + node->value);
//        }
//        else {
//            return std::atof(node->value.c_str());
//        }
//
//        throw std::runtime_error("Unknown operator or value");
//    }
//
//    bool isOperator(const std::string& value) const {
//        return value == "+" || value == "-" || value == "*" || value == "/" || value == "sin" || value == "cos";
//    }
//
//    bool isVariable(const std::string& value) const {
//        return std::isalpha(value[0]);
//    }
//
//public:
//    CTree() : root(NULL) {}
//
//    ~CTree() {
//        deleteTree(root);
//    }
//
//    void buildTree(const std::vector<std::string>& tokens) {
//        deleteTree(root); // Usuni?cie poprzedniego drzewa
//        std::vector<CNode*> stack;
//
//        try {
//            for (int i = tokens.size() - 1; i >= 0; --i) {
//                if (isOperator(tokens[i])) {
//                    CNode* node = new CNode(tokens[i]);
//
//                    // Okre?lamy liczb? argumentów operatora
//                    int requiredArgs = (tokens[i] == "sin" || tokens[i] == "cos") ? 1 : 2;
//
//                    // Sprawdzamy, czy stos zawiera wystarczaj?c? liczb? argumentów
//                    if (stack.size() < static_cast<size_t>(requiredArgs)) {
//                        throw std::runtime_error("Invalid formula: insufficient arguments for operator " + tokens[i]);
//                    }
//
//                    // Pobieramy wymagane argumenty ze stosu
//                    for (int j = 0; j < requiredArgs; ++j) {
//                        node->children.push_back(stack.back());
//                        stack.pop_back();
//                    }
//
//                    // Dodajemy w?ze? do stosu
//                    stack.push_back(node);
//                }
//                else {
//                    // Tworzymy li?? dla liczby lub zmiennej
//                    stack.push_back(new CNode(tokens[i]));
//                }
//            }
//
//            // Po przetworzeniu ca?ej formu?y stos powinien zawiera? dok?adnie jedno drzewo
//            if (stack.size() != 1) {
//                throw std::runtime_error("Invalid formula: too many elements remaining on stack");
//            }
//
//            root = stack.back(); // Ustawienie korzenia drzewa
//        }
//        catch (const std::exception& e) {
//            // Usuwamy wszystkie w?z?y, je?li wyst?pi? b??d
//            for (size_t i = 0; i < stack.size(); ++i) {
//                delete stack[i];
//            }
//            stack.clear();
//            throw; // Ponownie wyrzucamy wyj?tek
//        }
//    }
//
//
//
//    void print() const {
//        if (root) {
//            printNode(root);
//        }
//        std::cout << std::endl;
//    }
//
//    void vars() const {
//        std::vector<std::string> variables;
//        collectVars(root, variables);
//        for (size_t i = 0; i < variables.size(); ++i) {
//            std::cout << variables[i] << " ";
//        }
//        std::cout << std::endl;
//    }
//
//    void compute(const std::vector<double>& values) const {
//        std::vector<std::string> variables;
//        collectVars(root, variables);
//
//        if (values.size() != variables.size()) {
//            std::cerr << "Mismatch between number of variables and provided values" << std::endl;
//            return;
//        }
//
//        try {
//            double result = evaluateNode(root, variables, values);
//            std::cout << "Result: " << result << std::endl;
//        }
//        catch (const std::exception& e) {
//            std::cerr << e.what() << std::endl;
//        }
//    }
//};
//
//void parseCommand(const std::string& command, CTree& tree) {
//    std::istringstream iss(command);
//    std::string cmd;
//    iss >> cmd;
//
//    if (cmd == "enter") {
//        std::string formula;
//        getline(iss, formula);
//        std::istringstream formulaStream(formula);
//        std::vector<std::string> tokens;
//        std::string token;
//        while (formulaStream >> token) {
//            tokens.push_back(token);
//        }
//
//        try {
//            tree.buildTree(tokens);
//            std::cout << "Tree built successfully" << std::endl;
//        }
//        catch (const std::exception& e) {
//            std::cerr << "Error: " << e.what() << std::endl;
//        }
//    }
//    else if (cmd == "print") {
//        tree.print();
//    }
//    else if (cmd == "vars") {
//        tree.vars();
//    }
//    else if (cmd == "comp") {
//        std::vector<double> values;
//        double value;
//        while (iss >> value) {
//            values.push_back(value);
//        }
//        tree.compute(values);
//    }
//    else {
//        std::cerr << "Unknown command" << std::endl;
//    }
//}
//
//int main() {
//    CTree tree;
//    std::string command;
//
//    std::cout << "Enter commands (type 'exit' to quit):" << std::endl;
//    while (true) {
//        std::cout << "> ";
//        std::getline(std::cin, command);
//        if (command == "exit") break;
//        parseCommand(command, tree);
//    }
//
//    return 0;
//}
