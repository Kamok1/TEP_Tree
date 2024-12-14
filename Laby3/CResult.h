#ifndef CRESULT_H
#define CRESULT_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include "HasCopy.h"
#include "ISaver.h"

extern ISaver* globalSaver;

template <typename T, typename E>
class CResult {
public:
    CResult(const T& cValue);
    CResult(E* pcError);
    CResult(const T& cValue, std::vector<E*>& vErrors);
    CResult(const T& cValue, E* error);
    CResult(std::vector<E*>& vErrors);
    CResult(const CResult<T, E>& other);
    ~CResult();

    static CResult<T, E> cOk(const T& cValue);
    static CResult<T, E> cFail(E* pcError);
    static CResult<T, E> cFail(std::vector<E*>& vErrors);

    CResult<T, E>& operator=(const CResult<T, E>& other);

    bool bIsError() const;
    bool bIsSuccess() const;
    bool bIsSaved() const {
		return isSaved;
	}
    T cGetValue() const;
    const std::vector<E*>& vGetErrors() const;

private:
    T* pcValue;
    std::vector<E*> errors;
    mutable bool isSaved = false;
    void SetErrors(const std::vector<E*>& vErrors);
    void save() const;
};


template <typename T, typename E>
CResult<T, E>::CResult(const T& cValue) : pcValue(new T(cValue)) {
    save();
}

template <typename T, typename E>
CResult<T, E>::CResult(E* pcError) : pcValue(NULL) {
    errors.push_back(pcError);
    save();
}

template <typename T, typename E>
CResult<T, E>::CResult(const T& cValue, std::vector<E*>& vErrors) : pcValue(new T(cValue)), errors(vErrors) {
    save();
}

template <typename T, typename E>
CResult<T, E>::CResult(std::vector<E*>& vErrors) : pcValue(NULL), errors(vErrors) {
    save();
}

template <typename T, typename E>
CResult<T, E>::CResult(const CResult<T, E>& other) : pcValue(NULL), errors() {
    isSaved = other.isSaved;
    if (other.pcValue != NULL) {
        pcValue = new T(*(other.pcValue));
    }
    SetErrors(other.errors);
}

template <typename T, typename E>
CResult<T, E>::~CResult() {
    delete pcValue;
    for (int i = 0; i < errors.size(); ++i) {
        if(errors[i] != NULL)
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
        SetErrors(other.errors);
    }
    return *this;
}

template <typename T, typename E>
bool CResult<T, E>::bIsSuccess() const {
    return pcValue != NULL;
}

template <typename T, typename E>
bool CResult<T, E>::bIsError() const {
    return !errors.empty();
}

template <typename T, typename E>
T CResult<T, E>::cGetValue() const{
    return *pcValue;
}

template <typename T, typename E>
const std::vector<E*>& CResult<T, E>::vGetErrors() const {
    return errors;
}

template <typename T, typename E>
void CResult<T, E>::SetErrors(const std::vector<E*>& vErrors) {
    for (int i = 0; i < vErrors.size(); ++i) {
        if (vErrors[i] != nullptr) {
            if (HasCopy<E>::value) {
                errors.push_back(vErrors[i]->copy());
            }
            else {
                errors.push_back(new E(*vErrors[i]));
            }
        }
    }
}

template <typename T, typename E>
void CResult<T, E>::save() const {
    if (globalSaver && !isSaved) {
        globalSaver->save(const_cast<void*>(static_cast<const void*>(this)), typeid(*this));
        isSaved = true;
    }
}

#endif 
