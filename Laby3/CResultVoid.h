#ifndef CRESULTVOID_H
#define CRESULTVOID_H

#include <vector>
#include "CResult.h"
#include "CError.h"
using namespace std;

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

    bool bIsError() const;
    const std::vector<E*>& vGetErrors() const;

private:
    std::vector<E*> errors;
    void SetErrors(std::vector<E*> vErrors);
    bool isSaved = false;
    void save();
};

template <typename E>
CResult<void, E>::CResult() : errors() {}

template <typename E>
CResult<void, E>::CResult(E* pcError) : errors() {
    errors.push_back(pcError);
}

template <typename E>
CResult<void, E>::CResult(std::vector<E*>& vErrors) : errors() {
    SetErrors(vErrors);
}

template <typename E>
CResult<void, E>::CResult(const CResult<void, E>& cOther) : errors() {
    SetErrors(cOther.errors);
    isSaved = cOther.isSaved;
}


template <typename E>
CResult<void, E>::~CResult() {
    for(int i = 0; i < errors.size(); ++i) {
		delete errors[i];
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
    for(int i = 0; i < errors.size(); ++i) {
		delete errors[i];
	}
        errors.clear();
        SetErrors(cOther.errors);
    }
    return *this;
}

template <typename E>
bool CResult<void, E>::bIsError() const {
    return !errors.empty();
}

template <typename E>
const std::vector<E*>& CResult<void, E>::vGetErrors() const {
    return errors;
}

template <typename E>
void CResult<void, E>::SetErrors(std::vector<E*> vErrors) {
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

template <typename E>
void CResult<void, E>::save() {
    if (saverMangager && !isSaved && typeid(E) == typeid(CError)) {
        isSaved = saverMangager->save(static_cast<const void*>(this), typeid(void*));
    }
}
#endif
