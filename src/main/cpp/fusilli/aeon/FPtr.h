#if !defined(__FPTR_H__)
#define __FPTR_H__

// from stdc++
#include "fusilli.h"
NS_BEGIN(fusii)
//////////////////////////////////////////////////////////////////////////////
//
template<typename _Tp1> struct FS_DLL FPtr_ref {
  _Tp1 *_M_ptr;
  explicit
  FPtr_ref(_Tp1 *__p) : _M_ptr(__p) { }
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename _Tp> class FS_DLL FPtr {
private:
  _Tp *_M_ptr;

public:

  typedef _Tp pType;

  explicit
  FPtr(pType *__p = nullptr) throw() : _M_ptr(__p) { }

  FPtr(FPtr &__a) throw() : _M_ptr(__a.release()) { }

  template<typename _Tp1>
  FPtr(FPtr<_Tp1> &__a) throw() : _M_ptr(__a.release()) { }

  FPtr&
  operator=(FPtr &__a) throw() {
    reset(__a.release());
    return *this;
  }

  template<typename _Tp1>
  FPtr&
  operator=(FPtr<_Tp1> &__a) throw() {
    reset(__a.release());
    return *this;
  }

  ~FPtr() { delete _M_ptr; }

  pType&
  operator*() const throw() {
    assert(_M_ptr != nullptr);
    return *_M_ptr;
  }

  pType*
  operator->() const throw() {
    assert(_M_ptr != nullptr);
    return _M_ptr;
  }

  pType* get() const throw() { return _M_ptr; }

  pType* release() throw() {
    pType *__tmp = _M_ptr;
    _M_ptr = nullptr;
    return __tmp;
  }

  void reset(pType* __p = 0) throw() {
    if (__p != _M_ptr) {
      delete _M_ptr;
      _M_ptr = __p;
    }
  }

  FPtr(FPtr_ref<pType> __ref) throw()
  : _M_ptr(__ref._M_ptr) { }

  FPtr&
  operator=(FPtr_ref<pType> __ref) throw() {
    if (__ref._M_ptr != this->get()) {
      delete _M_ptr;
      _M_ptr = __ref._M_ptr;
    }
    return *this;
  }

  template<typename _Tp1>
  operator FPtr_ref<_Tp1>() throw() {
    return FPtr_ref<_Tp1>(this->release());
  }

  template<typename _Tp1>
  operator FPtr<_Tp1>() throw() {
    return FPtr<_Tp1>(this->release()); }

};


NS_END(fusii)
#endif

