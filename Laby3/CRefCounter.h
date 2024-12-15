#ifndef REF_COUNTER_H
#define REF_COUNTER_H
template <typename T>
class CRefCounter {
public:
    CRefCounter() : count(1) {}
    void addRef() { ++count; }
    int releaseRef() { return --count; }
    int getRefCount() const { return count; }

private:
    int count;
};
#endif