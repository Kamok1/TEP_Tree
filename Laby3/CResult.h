#ifndef CRESULT_H
#define CRESULT_H

#include <vector>
#include <stdexcept>
#include <iostream>

template <typename T, typename E>
class CResult {
public:
    CResult(const T& cValue);
    CResult(E* pcError);
    CResult(std::vector<E*>& vErrors);
    CResult(const CResult<T, E>& other);
    ~CResult();

    static CResult<T, E> cOk(const T& cValue);
    static CResult<T, E> cFail(E* pcError);
    static CResult<T, E> cFail(std::vector<E*>& vErrors);

    CResult<T, E>& operator=(const CResult<T, E>& other);

    bool bIsSuccess() const;
    T cGetValue() const;
    const std::vector<E*>& vGetErrors() const;

private:
    T* pcValue;
    std::vector<E*> errors;
};


template <typename T, typename E>
CResult<T, E>::CResult(const T& cValue) : pcValue(new T(cValue)) {}

template <typename T, typename E>
CResult<T, E>::CResult(E* pcError) : pcValue(NULL) {
    errors.push_back(pcError);
}

template <typename T, typename E>
CResult<T, E>::CResult(std::vector<E*>& vErrors) : pcValue(NULL), errors(vErrors) {}

template <typename T, typename E>
CResult<T, E>::CResult(const CResult<T, E>& other) : pcValue(NULL), errors() {
    if (other.pc_value != NULL) {
        pcValue = new T(*(other.pcValue));
    }
    for (int i = 0; i < other.errors.size(); ++i) {
        errors.push_back(new E(*(other.errors[i])));
    }
}

template <typename T, typename E>
CResult<T, E>::~CResult() {
    delete pcValue;
    for (int i = 0; i < errors.size(); ++i) {
        delete errors[i];
    }
}

template <typename T, typename E>
CResult<T, E> CResult<T, E>::cOk(const T& cValue) {
    return CResult<T, E>(cValue);
}

template <typename T, typename E>
CResult<T, E> CResult<T, E>::cFail(E* pcError) {
    return CResult<T, E>(pcError);
}

template <typename T, typename E>
CResult<T, E> CResult<T, E>::cFail(std::vector<E*>& vErrors) {
    return CResult<T, E>(vErrors);
}

template <typename T, typename E>
CResult<T, E>& CResult<T, E>::operator=(const CResult<T, E>& other) {
    if (this != &other) {
        delete pcValue;
        pcValue = other.pcValue ? new T(*other.pcValue) : NULL;

        for (int i = 0; i < errors.size(); ++i) {
            delete errors[i];
        }

        for (int i = 0; i < other.errors.size(); ++i) {
            errors.push_back(new E(*(other.errors[i])));
        }
    }
    return *this;
}

template <typename T, typename E>
bool CResult<T, E>::bIsSuccess() const {
    return pcValue != NULL;
}

template <typename T, typename E>
T CResult<T, E>::cGetValue() const{
    return *pcValue;
}

template <typename T, typename E>
const std::vector<E*>& CResult<T, E>::vGetErrors() const {
    return errors;
}

#endif 
