#ifndef MY_SMART_POINTER_H
#define MY_SMART_POINTER_H
#include "CRefCounter.h"

template <typename T>
class CMySmartPointer {
public:
    CMySmartPointer(T* pointer = nullptr)
        : pc_pointer(pointer), pc_counter(pointer ? new CRefCounter<T>() : nullptr) {}

    CMySmartPointer(const CMySmartPointer& other)
        : pc_pointer(other.pc_pointer), pc_counter(other.pc_counter) {
        if (pc_counter) {
            pc_counter->addRef();
        }
    }

    CMySmartPointer(CMySmartPointer&& other)
        : pc_pointer(other.pc_pointer), pc_counter(other.pc_counter) {
        other.pc_pointer = nullptr;
        other.pc_counter = nullptr;
    }

    CMySmartPointer& operator=(const CMySmartPointer& other) {
        if (this != &other) {
            release();
            pc_pointer = other.pc_pointer;
            pc_counter = other.pc_counter;
            if (pc_counter) {
                pc_counter->addRef();
            }
        }
        return *this;
    }

    CMySmartPointer& operator=(CMySmartPointer&& other) {
        if (this != &other) {
            release();
            pc_pointer = other.pc_pointer;
            pc_counter = other.pc_counter;
            other.pc_pointer = nullptr;
            other.pc_counter = nullptr;
        }
        return *this;
    }

    T* get() const {
        return pc_pointer;
    }

    ~CMySmartPointer() { release(); }

    T& operator*() const { return *pc_pointer; }
    T* operator->() const { return pc_pointer; }

private:
    T* pc_pointer;
    CRefCounter<T>* pc_counter;

    void release() {
        if (pc_counter && pc_counter->releaseRef() == 0) {
            delete pc_pointer;
            delete pc_counter;
        }
    }
};

#endif