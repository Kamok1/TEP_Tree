#include "CNode.h"

CNode::CNode(const std::string& val, int count): value(val), childCount(count) {
    children = new CNode * [childCount];
    for (int i = 0; i < childCount; ++i) {
        children[i] = nullptr;
    }
}

CNode::~CNode() {
    for (int i = 0; i < childCount; ++i) {
        if (children[i]) {
            delete children[i];
            children[i] = nullptr;
        }
    }
    delete[] children;
}



void CNode::setChild(int index, CNode* child) {
    if (index >= 0 && index < childCount) {
        children[index] = child;
    }
}

const std::string& CNode::getValue() const {
    return value;
}

int CNode::getChildCount() const {
    return childCount;
}

CNode* CNode::getChild(int index) const {
    return index >= 0 && index < childCount ? children[index] : nullptr;
}