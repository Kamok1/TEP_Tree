#include "CTree.h"

CTree::CTree() : root(NULL) {}

CTree::~CTree() {
    deleteTree(root);
}

CTree::CTree(const CTree& other) : root(NULL) {
    if (other.root)
        root = copySubtree(other.root);
}

CNode* CTree::copySubtree(CNode* node) const{
    if (!node) 
        return NULL;

    CNode* newNode = new CNode(node->getValue(), node->getChildCount());

    for (int i = 0; i < node->getChildCount(); ++i) {
        newNode->setChild(i, copySubtree(node->getChild(i)));
    }

    return newNode;
}

CTree& CTree::operator=(const CTree& other) {
    if (this != &other) {
        deleteTree(root);
        root = copySubtree(other.root);
    }
    return *this;
}

CTree CTree::operator+(const CTree& other) const {
    CTree newTree;
    if (root && other.root) {
        newTree.root = replaceLeafWithSubtree(root, other.root);
    }
    else if (root) {
        newTree.root = copySubtree(root);
    }
    else if (other.root) {
        newTree.root = copySubtree(other.root);
    }
    return newTree;
}

CNode* CTree::replaceLeafWithSubtree(CNode* node, CNode* subtree) const {
    bool isReplaced = false;
    return replaceLeafWithSubtree(node, subtree, isReplaced);
}

CNode* CTree::replaceLeafWithSubtree(CNode* node, CNode* subtree, bool& isReplaced) const {
    if (!node) 
        return NULL;

    if (node->getChildCount() == 0 && !isReplaced) {
        isReplaced = true;
        return copySubtree(subtree);
    }

    CNode* newNode = new CNode(node->getValue(), node->getChildCount());
    for (int i = 0; i < node->getChildCount(); ++i) {
        newNode->setChild(i, replaceLeafWithSubtree(node->getChild(i), subtree, isReplaced));
    }

    return newNode;
}

void CTree::deleteTree(CNode* node) {
    if (!node) 
        return;

    for (int i = 0; i < node->getChildCount(); ++i) {
        CNode* child = node->getChild(i);
        if (child) {
            deleteTree(child);
            node->setChild(i, NULL);
        }
    }
    delete node;
}

void CTree::getNodes(CNode* node, std::vector<std::string>& values) const {
    if (!node) 
        return;
    values.push_back(node->getValue());
    for (int i = 0; i < node->getChildCount(); ++i) {
        getNodes(node->getChild(i), values);
    }
}

void CTree::getVars(CNode* node, std::vector<std::string>& vars) const {
    if (!node) 
        return;
    if (isVariable(node->getValue())) {
        if (std::find(vars.begin(), vars.end(), node->getValue()) == vars.end()) {
            vars.push_back(node->getValue());
        }
    }
    for (int i = 0; i < node->getChildCount(); ++i) {
        getVars(node->getChild(i), vars);
    }
}

double CTree::evaluateNode(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values, std::string& errorMsg) const {
    if (!node) {
        errorMsg = "Invalid node";
        return -DBL_MAX;
    }

    std::string nodeValue = node->getValue();

    if (isOperator(nodeValue)) 
        return OperateOnOperator(node, vars, values, errorMsg);
    else if (isVariable(nodeValue)) 
        return OperateOnVariable(nodeValue, vars, values, errorMsg);
    return std::atof(nodeValue.c_str());
}

double CTree::OperateOnOperator(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values, std::string& errorMsg) const {
    std::string nodeValue = node->getValue();
    int childCount = node->getChildCount();
    int requiredArgs = getRequiredArgs(nodeValue);


    if (nodeValue == SIN) 
        return std::sin(evaluateNode(node->getChild(0), vars, values, errorMsg));
    if (nodeValue == COS) 
        return std::cos(evaluateNode(node->getChild(0), vars, values, errorMsg));


    double result = (nodeValue == SUM || nodeValue == SUB || nodeValue == MAX4) ? DEFAULT_VALUE_FOR_BASIC_OPERATIONS : DEFAULT_VALUE_FOR_COMPLEX_OPERATIONS;
    for (int i = 0; i < childCount; ++i) {
        CNode* child = node->getChild(i);
        double childResult = evaluateNode(child, vars, values, errorMsg);

        if (!errorMsg.empty())
            return -DBL_MAX;

        if (nodeValue == MAX4)
            result = result > childResult ? result : childResult;

        else if (nodeValue == SUM) 
            result += childResult;

        else if (nodeValue == MUL)
            result *= childResult;

        else if (nodeValue == SUB)
            result = (i == 0) ? childResult : result - childResult;
        
        else if (nodeValue == DIV) {
            if (i > 0 && childResult == 0) {
                errorMsg = "Division by zero";
                return -DBL_MAX;
            }
            result = (i == 0) ? childResult : result / childResult;
        }
        else {
            errorMsg = "Unsupported operator: " + nodeValue;
            return -DBL_MAX;
        }
    }

    return result;
}


double CTree::OperateOnVariable(const std::string& nodeValue, const std::vector<std::string>& vars, const std::vector<double>& values, std::string& errorMsg) const {
    for (int i = 0; i < vars.size(); ++i) {
        if (vars[i] == nodeValue) {
            return values[i];
        }
    }
    errorMsg = "Variable '" + nodeValue + "' not found in provided values";
    return -DBL_MAX;
}


bool CTree::isOperator(const std::string& value) const {
    return value == SUM || value == SUB || value == MUL || value == DIV || value == SIN || value == COS || value == MAX4;
}

bool CTree::isVariable(const std::string& value) const {
    return value != SIN && value != COS && value != MAX4 && std::isalpha(value[0]);
}

std::string CTree::sanitizeVariable(const std::string& variable, std::string& message) const {
    std::string sanitized;

    for (char ch : variable) {
        std::isalnum(ch) ? sanitized += ch : message += "Ignoring invalid character in variable: " + std::string(1, ch) + "\n";
    }
    return sanitized;
}

CNode* CTree::buildSubtree(std::istringstream& stream, std::string& message, bool hasToBeOperator) {
    std::string token;
    if (!(stream >> token)) {
        message += "Unexpected end of expression \n";
        token = DEFAULT_NODE_VALUE;
    }

    bool isTokenOperator = isOperator(token);
    if (isTokenOperator == false && hasToBeOperator)
    {
        message += "Expected operator";
        return new CNode(token, 0);
    }


    if (isTokenOperator) {
        int requiredArgs = getRequiredArgs(token);
        CNode* node = new CNode(token, requiredArgs);

		for (int i = 0; i < requiredArgs; ++i) {
            bool hasToBeOperator = (token == MAX4 && (i == 0 || i == 3));
            CNode* child = buildSubtree(stream, message, hasToBeOperator);
            node->setChild(i, child);
        }
        return node;
    }
    else if (isVariable(token)) return new CNode(sanitizeVariable(token, message), 0);
    return new CNode(token, 0);
}

int CTree::getRequiredArgs(const std::string& operatorToken) const {
    if (operatorToken == SIN || operatorToken == COS) 
        return DEFAULT_SIN_COS_CHILDREN;
    if (operatorToken == SUM || operatorToken == SUB || operatorToken == MUL || operatorToken == DIV) 
        return DEFAULT_OPERATOR_CHILDREN;
    if (operatorToken == MAX4)
        return 4;
    return 0;
}


void CTree::buildTree(std::istringstream& stream, std::string& message) {
    deleteTree(root);
    root = buildSubtree(stream, message, false);

    std::string leftover;
    if (stream >> leftover)
        message += "Unexpected token: " + leftover + "\n";
}

void CTree::getTreeNodeValues(std::vector<std::string>& values) const {
    if (root) 
        getNodes(root, values);
}

void CTree::getTreeVars(std::vector<std::string>& vars) const {
    if (root) 
        getVars(root, vars);
}

void CTree::compute(double& result ,const std::vector<double>& values, const std::vector<std::string>& vars, std::string& message) const {
    result = evaluateNode(root, vars, values, message);
}
