#include "CNode.h"

CNode::CNode(const std::string& val, int count): value(val), childCount(count) {
    children = new CNode * [childCount];
    for (int i = 0; i < childCount; ++i) {
        children[i] = NULL;
    }
}

CNode::~CNode() {
    for (int i = 0; i < childCount; ++i) {
        if (children[i]) {
            delete children[i];
            children[i] = NULL;
        }
    }
    delete[] children;
}


void CNode::setChild(int index, CNode* child) {
    if (index >= 0 && index < childCount) children[index] = child;
}

const std::string& CNode::getValue() const {
    return value;
}

int CNode::getChildCount() const {
    return childCount;
}

CNode* CNode::getChild(int index) const {
    return index >= 0 && index < childCount ? children[index] : NULL;
}