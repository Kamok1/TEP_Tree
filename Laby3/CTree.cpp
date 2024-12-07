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

CResult<double, CError> CTree::evaluateNode(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values) const {
    if (!node) {
        return new CError(ERROR_NULL_REFERENCE, ERROR, "Invalid node");
    }

    std::string nodeValue = node->getValue();

    if (isOperator(nodeValue)) 
        return OperateOnOperator(node, vars, values);
    else if (isVariable(nodeValue)) 
        return OperateOnVariable(nodeValue, vars, values);
    return std::atof(nodeValue.c_str());
}

CResult<double, CError> CTree::OperateOnOperator(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values) const {
    std::string nodeValue = node->getValue();
    int childCount = node->getChildCount();
    int requiredArgs = getRequiredArgs(nodeValue);
    double result = (nodeValue == SUM || nodeValue == SUB || nodeValue == MAX4) ? DEFAULT_VALUE_FOR_BASIC_OPERATIONS : DEFAULT_VALUE_FOR_COMPLEX_OPERATIONS;
    for (int i = 0; i < childCount; ++i) {
        CNode* child = node->getChild(i);
        CResult<double, CError> childResult = evaluateNode(child, vars, values);

        if (childResult.bIsSuccess() == false)
            return childResult;

        double childResultValue = childResult.cGetValue();

        if (nodeValue == SIN)
            result = std::sin(childResultValue);

        else if (nodeValue == COS)
            result = std::cos(childResultValue);

        else if (nodeValue == MAX4)
            result = result > childResultValue ? result : childResultValue;

        else if (nodeValue == SUM)
            result += childResultValue;

        else if (nodeValue == MUL)
            result *= childResultValue;

        else if (nodeValue == SUB)
            result = (i == 0) ? childResultValue : result - childResultValue;

        else if (nodeValue == DIV) {
            if (i > 0 && childResultValue == 0) {
                return new CError(ERROR_DIVISION_BY_ZERO, ERROR, "Division by zero");
            }
            result = (i == 0) ? childResultValue : result / childResultValue;
        }
        else {
            return new CError(ERROR_INVALID_INPUT, ERROR, "Unsupported operator: " + nodeValue);
        }
    }
    return result;
}


CResult<double, CError> CTree::OperateOnVariable(const std::string& nodeValue, const std::vector<std::string>& vars, const std::vector<double>& values) const {
    for (int i = 0; i < vars.size(); ++i) {
        if (vars[i] == nodeValue) {
            return values[i];
        }
    }
    return new CError(ERROR_INVALID_INPUT, ERROR, "Variable '" + nodeValue + "' not found in provided values");
}


bool CTree::isOperator(const std::string& value) const {
    return value == SUM || value == SUB || value == MUL || value == DIV || value == SIN || value == COS || value == MAX4;
}

bool CTree::isVariable(const std::string& value) const {
    return value != SIN && value != COS && value != MAX4 && std::isalpha(value[0]);
}

CResult<std::string, CError> CTree::sanitizeVariable(const std::string& variable) const {
    std::string sanitized;
    std::vector<CError*> errors;

    for (char ch : variable) {
        if (std::isalnum(ch)) {
			sanitized += ch;
		}
        else {
            errors.push_back(new CError(ERROR_INVALID_INPUT, INFO, "Ignoring invalid character in variable: " + std::string(1, ch)));
        }
    }
    return CResult<std::string, CError>(sanitized, errors);
}

CResult<CNode*, CError> CTree::buildSubtree(std::istringstream& stream, bool hasToBeOperator) {

    std::string token;
    std::vector<CError*> accumulatedErrors;

    if (!(stream >> token)) {
        accumulatedErrors.push_back(new CError(ERROR_INVALID_INPUT, INFO, "Unexpected end of expression"));
        token = DEFAULT_NODE_VALUE;
    }

    bool isTokenOperator = isOperator(token);
    if (isTokenOperator == false && hasToBeOperator)
        return new CError(ERROR_INVALID_INPUT, ERROR, "Expected operator");

    CNode* node = new CNode(token, isTokenOperator ? getRequiredArgs(token) : 0);


    if (isTokenOperator) {
        for (int i = 0; i < node->getChildCount(); ++i) {
            bool childHasToBeOperator = (token == MAX4 && (i == 0 || i == 3));
            CResult<CNode*, CError> childResult = buildSubtree(stream, childHasToBeOperator);
            node->setChild(i, childResult.cGetValue());
            if (childResult.bIsError())
            {
                const std::vector<CError*>& errors = childResult.vGetErrors();
                for (std::vector<CError*>::const_iterator it = errors.begin(); it != errors.end(); ++it) {
                    for (std::vector<CError*>::const_iterator it = errors.begin(); it != errors.end(); ++it) {
                        accumulatedErrors.push_back(new CError(**it));
                    }
                }
            }
        }
    }
    else if (isVariable(token)) 
    {
        CResult<std::string, CError> sanitizedResult = sanitizeVariable(token);
        if (!sanitizedResult.bIsSuccess())
        {
            const std::vector<CError*>& errors = sanitizedResult.vGetErrors();
            for (std::vector<CError*>::const_iterator it = errors.begin(); it != errors.end(); ++it) {
                for (std::vector<CError*>::const_iterator it = errors.begin(); it != errors.end(); ++it) {
                    accumulatedErrors.push_back(new CError(**it));
                }
            }
        }
        node->setValue(sanitizedResult.cGetValue());
    }
    return CResult<CNode*, CError>(node, accumulatedErrors);
}

int CTree::getRequiredArgs(const std::string& operatorToken) const {
    if (operatorToken == SIN || operatorToken == COS) 
        return DEFAULT_SIN_COS_CHILDREN;
    if (operatorToken == SUM || operatorToken == SUB || operatorToken == MUL || operatorToken == DIV) 
        return DEFAULT_OPERATOR_CHILDREN;
    if (operatorToken == MAX4)
        return DEFAULT_MAX4_CHILDREN;
    return DEFAULT_CHILDREN;
}


CResult<void, CError> CTree::buildTree(std::istringstream& stream) {
    deleteTree(root);
    CResult<CNode*, CError> result = buildSubtree(stream, false);
    if (result.bIsSuccess())
        root = result.cGetValue();

    std::vector<CError*> errors = result.vGetErrors();
    std::string leftover;
    if (stream >> leftover)
        errors.push_back(new CError(ERROR_INVALID_INPUT, INFO, "Ignoring leftover: " + leftover));
    return CResult<void, CError>(errors);
}

void CTree::getTreeNodeValues(std::vector<std::string>& values) const {
    if (root) 
        getNodes(root, values);
}

void CTree::getTreeVars(std::vector<std::string>& vars) const {
    if (root) 
        getVars(root, vars);
}

CResult<double, CError> CTree::compute(const std::vector<double>& values, const std::vector<std::string>& vars) const {
     return evaluateNode(root, vars, values);
}
