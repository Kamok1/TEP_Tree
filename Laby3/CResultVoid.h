#ifndef CRESULTVOID_H
#define CRESULTVOID_H

#include <vector>
#include "CResult.h"
#include "CError.h"

template <typename E>
class CResult<void, E> {
public:
    CResult();
    CResult(E* pcError);
    CResult(std::vector<E*>& vErrors);
    CResult(const CResult<void, E>& cOther);
    ~CResult();

    static CResult<void, E> cOk();
    static CResult<void, E> cFail(E* pcError);
    static CResult<void, E> cFail(std::vector<E*>& vErrors);

    CResult<void, E>& operator=(const CResult<void, E>& cOther);

    bool bIsSuccess() const;
    const std::vector<E*>& vGetErrors() const;

private:
    std::vector<E*> errors;
};

template <typename E>
CResult<void, E>::CResult() : errors() {}

template <typename E>
CResult<void, E>::CResult(E* pcError) : errors() {
    if (pcError != nullptr) {
        errors.push_back(pcError);
    }
}

template <typename E>
CResult<void, E>::CResult(std::vector<E*>& vErrors) : errors() {
    for (auto& error : vErrors) {
        if (error != nullptr) {
            errors.push_back(new E(*error));
        }
    }
}

template <typename E>
CResult<void, E>::CResult(const CResult<void, E>& cOther) : errors() {
    for (auto& error : cOther.errors) {
        if (error != nullptr) {
            errors.push_back(new E(*error));
        }
    }
}

template <typename E>
CResult<void, E>::~CResult() {
    for (auto& error : errors) {
        delete error;
    }
}

template <typename E>
CResult<void, E> CResult<void, E>::cOk() {
    return CResult<void, E>();
}

template <typename E>
CResult<void, E> CResult<void, E>::cFail(E* pcError) {
    return CResult<void, E>(pcError);
}

template <typename E>
CResult<void, E> CResult<void, E>::cFail(std::vector<E*>& vErrors) {
    return CResult<void, E>(vErrors);
}

template <typename E>
CResult<void, E>& CResult<void, E>::operator=(const CResult<void, E>& cOther) {
    if (this != &cOther) {
        for (auto& error : errors) {
            delete error;
        }
        errors.clear();
        for (auto& error : cOther.errors) {
            if (error != nullptr) {
                errors.push_back(new E(*error));
            }
        }
    }
    return *this;
}

template <typename E>
bool CResult<void, E>::bIsSuccess() const {
    return errors.empty();
}

template <typename E>
const std::vector<E*>& CResult<void, E>::vGetErrors() const {
    return errors;
}

#endif
