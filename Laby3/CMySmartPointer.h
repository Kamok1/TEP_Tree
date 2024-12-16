#ifndef MY_SMART_POINTER_H
#define MY_SMART_POINTER_H

#include "CRefCounter.h"

template <typename T>
class CMySmartPointer {
public:
    CMySmartPointer(T* pointer = nullptr);
    CMySmartPointer(const CMySmartPointer& other);
    CMySmartPointer(CMySmartPointer&& other);

    CMySmartPointer& operator=(const CMySmartPointer& other);
    CMySmartPointer& operator=(CMySmartPointer&& other);

    ~CMySmartPointer();

    T* get() const;

    T& operator*() const;
    T* operator->() const;

private:
    T* pc_pointer;
    CRefCounter<T>* pc_counter;

    void release();
};


template <typename T>
CMySmartPointer<T>::CMySmartPointer(T* pointer)
    : pc_pointer(pointer), pc_counter(pointer ? new CRefCounter<T>() : nullptr) {}

template <typename T>
CMySmartPointer<T>::CMySmartPointer(const CMySmartPointer& other)
    : pc_pointer(other.pc_pointer), pc_counter(other.pc_counter) {
    if (pc_counter) {
        pc_counter->addRef();
    }
}

template <typename T>
CMySmartPointer<T>::CMySmartPointer(CMySmartPointer&& other)
    : pc_pointer(other.pc_pointer), pc_counter(other.pc_counter) {
    other.pc_pointer = nullptr;
    other.pc_counter = nullptr;
}

template <typename T>
CMySmartPointer<T>& CMySmartPointer<T>::operator=(const CMySmartPointer& other) {
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

template <typename T>
CMySmartPointer<T>& CMySmartPointer<T>::operator=(CMySmartPointer&& other) {
    if (this != &other) {
        release();
        pc_pointer = other.pc_pointer;
        pc_counter = other.pc_counter;
        other.pc_pointer = nullptr;
        other.pc_counter = nullptr;
    }
    return *this;
}

template <typename T>
T* CMySmartPointer<T>::get() const {
    return pc_pointer;
}

template <typename T>
CMySmartPointer<T>::~CMySmartPointer() {
    release();
}

template <typename T>
T& CMySmartPointer<T>::operator*() const {
    return *pc_pointer;
}

template <typename T>
T* CMySmartPointer<T>::operator->() const {
    return pc_pointer;
}

template <typename T>
void CMySmartPointer<T>::release() {
    if (pc_counter && pc_counter->releaseRef() == 0) {
        delete pc_pointer;
        delete pc_counter;
    }
}

#endif
