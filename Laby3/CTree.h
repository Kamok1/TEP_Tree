#ifndef CTREE_H
#define CTREE_H

#include "CNode.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cfloat> 
#include "CError.h"
#include "CResult.h"
#include "CResultVoid.h"
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
    int getRequiredArgs(const std::string& operatorToken) const;

public:
    CTree();
    ~CTree();
    CTree(const CTree& other);

    CTree& operator=(const CTree& other);
    CTree operator+(const CTree& other) const;

    CResult<void, CError> buildTree(std::istringstream& stream);
    void getTreeNodeValues(std::vector<std::string>& values) const;
    void getTreeVars(std::vector<std::string>& vars) const;
    CResult<double, CError> compute(const std::vector<double>& values, const std::vector<std::string>& vars) const;
};

#endif