#ifndef CNODE_H
#define CNODE_H

#include <string>

class CNode {
private:
    std::string value;
    CNode** children;
    int childCount;

public:
    CNode(const std::string& val, int count);
    ~CNode();

    void setChild(int index, CNode* child);

    const std::string& getValue() const;
    int getChildCount() const;
    CNode* getChild(int index) const;
};

#endif
