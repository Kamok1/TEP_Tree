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
#define DEFAULT_NODE_VALUE "1"
#define DEFAULT_SIN_COS_CHILDREN 1
#define DEFAULT_OPERATOR_CHILDREN 2
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

    double evaluateNode(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values, std::string& errorMsg) const;
    double OperateOnOperator(CNode* node, const std::vector<std::string>& vars, const std::vector<double>& values, std::string& errorMsg) const;
    double OperateOnVariable(const std::string& nodeValue, const std::vector<std::string>& vars, const std::vector<double>& values, std::string& errorMsg) const;
    CNode* buildSubtree(std::istringstream& stream, std::string& message, bool hasToBeOperator);
    bool isOperator(const std::string& value) const;
    bool isVariable(const std::string& value) const;
    std::string sanitizeVariable(const std::string& variable, std::string& message) const;
    CNode* replaceLeafWithSubtree(CNode* leaf, CNode* subtree) const;
    CNode* replaceLeafWithSubtree(CNode* leaf, CNode* subtree, bool& replaced) const;
    int getRequiredArgs(const std::string& operatorToken) const;

public:
    CTree();
    ~CTree();
    CTree(const CTree& other);

    CTree& operator=(const CTree& other);
    CTree operator+(const CTree& other) const;

    void buildTree(std::istringstream& stream, std::string& message);
    void getTreeNodeValues(std::vector<std::string>& values) const;
    void getTreeVars(std::vector<std::string>& vars) const;
    void compute(double& result, const std::vector<double>& values, const std::vector<std::string>& vars, std::string& message) const;
};

#endif