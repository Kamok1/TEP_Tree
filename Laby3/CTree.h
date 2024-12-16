#ifndef CTREE_H
#define CTREE_H

#include "CNode.h"
#include "CResultVoid.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cfloat> 
#define DEFAULT_NODE_VALUE "1"
#define DEFAULT_SIN_COS_CHILDREN 1
#define DEFAULT_OPERATOR_CHILDREN 2
#define DEFAULT_MAX4_CHILDREN 4
#define DEFAULT_CHILDREN 0
#define DEFAULT_VALUE_FOR_BASIC_OPERATIONS 0
#define DEFAULT_VALUE_FOR_COMPLEX_OPERATIONS 1
#define SUM "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"
#define SIN "sin"
#define COS "cos"
#define MAX4 "MAX4"
#define LEFTOVER_MESSAGE "Ignoring leftover"
#define EXPECTED_OPERATOR_MESSAGE "Expected operator"
#define UNEXPECTED_EXPRESSION_END_MESSAGE "Unexpected end of expression"
#define IGNORING_INVALID_CHAR_MESSAGE "Ignoring invalid character in variable: "
#define ZERO_DIVISION_MESSAGE "Division by zero"
#define UNSUPPORTED_OPERATOR_MESSAGE "Unsupported operator: "
#define VARIABLE_NOT_FOUND_MESSAGE "Variable not found in provided values: " 
#define INVALID_NODE_MESSAGE "Invalid node"
#define MISSMATCHED_VARIABLES_AND_VALUES_MESSAGE "Mismatch between number of variables and provided values"

class CTree {
private:
    CNode* root;
    void deleteTree(CNode* node);
    void fixExpression(CNode* node);
    void getNodes(CNode* node, std::vector<std::string>& values) const;
    void getVars(CNode* node, std::vector<std::string>& vars) const;
    CNode* copySubtree(CNode* node) const;

    CResult<double, CError> evaluateNode(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values) const;
    CResult<double, CError> OperateOnOperator(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values) const;
    CResult<double, CError> OperateOnVariable(const std::string& nodeValue, const std::vector<std::string>& vars, const std::vector<double>& values) const;
    CResult<CNode*, CError> buildSubtree(std::istringstream& stream, bool hasToBeOperator);
    bool isOperator(const std::string& value) const;
    bool isVariable(const std::string& value) const;
    CResult<std::string, CError> sanitizeVariable(const std::string& variable) const;
    CNode* replaceLeafWithSubtree(CNode* leaf, CNode* subtree) const;
    CNode* replaceLeafWithSubtree(CNode* leaf, CNode* subtree, bool& replaced) const;

    std::string buildPrefix(CNode* node) const;
    int getRequiredArgs(const std::string& operatorToken) const;
    void addErrors(std::vector<CError*>& errors, const std::vector<CError*>& newErrors) const;

public:
    CTree();
    ~CTree();
    CTree(const CTree& other);
    CTree(CTree&& other);

    CTree& operator=(CTree&& other);
    CTree& operator=(const CTree& other);
    CTree operator+(const CTree& other) const;

    CResult<void, CError> buildTree(std::istringstream& stream);
    CResult<string, CError> getPrefix() const;
    void getTreeNodeValues(std::vector<std::string>& values) const;
    void getTreeVars(std::vector<std::string>& vars) const;
    CResult<double, CError> compute(const std::vector<double>& values, const std::vector<std::string>& vars) const;
};

#endif