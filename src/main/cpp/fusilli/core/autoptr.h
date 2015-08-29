// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#if !defined(__AUTOPTR_H__)
#define __AUTOPTR_H__

#include "fusilli.h"

//////////////////////////////////////////////////////////////////////////////
// macros to handle auto-naming
#define mc_autoptr(Class, aPtr) TautoPtr <Class> autoptr_##aPtr(aPtr)
#define mc_rel_ptr(aPtr)   autoptr_##aPtr.Disown()
class FI_DLLEXPORT TfilePtr {
public:

  virtual ~TfilePtr() { Destroy(); }

  TfilePtr& operator=( FILE*& fp) {
    m_fp = &fp;
    return *this;
  }

  TfilePtr( FILE*& fp) {
    m_fp = &fp;
  }

  FILE * Ptr()  { return m_fp ? *m_fp : nullptr; }
  void Disown() { m_fp = nullptr; }
  void Destroy() { if (m_fp) { mc_free_fp(*m_fp); } }

private:

  DISALLOW_COPY_AND_ASSIGN(TfilePtr)
  FILE**  m_fp;
};

//////////////////////////////////////////////////////////////////////////////
//
class FI_DLLEXPORT TmemPtr {
public:

  virtual ~TmemPtr() { Destroy(); }

  TmemPtr( void*& p) { m_ptr = &p; }

  TmemPtr& operator=( void*& p) {
    m_ptr = &p;
    return *this;
  }

  void*  Ptr()    { return m_ptr ? *m_ptr : nullptr; }
  void  Disown()  { m_ptr = nullptr; }
  void  Destroy()  { if ( m_ptr) { mc_free_mem(*m_ptr);} }

private:

  DISALLOW_COPY_AND_ASSIGN(TmemPtr)
  void**  m_ptr;
};

//////////////////////////////////////////////////////////////////////////////
//
class FI_DLLEXPORT TstrPtr {
public:

  virtual ~TstrPtr() { Destroy(); }

  TstrPtr( char*& p) { m_ptr = &p; }

  TstrPtr& operator=( char*& p) {
    m_ptr = &p;
    return *this;
  }

  char*  Ptr()    { return m_ptr ? *m_ptr : nullptr; }
  void  Disown()  { m_ptr = nullptr; }
  void  Destroy()  { if ( m_ptr) { mc_free_str(*m_ptr);} }

private:

  DISALLOW_COPY_AND_ASSIGN(TstrPtr)
  char**  m_ptr;
};

//////////////////////////////////////////////////////////////////////////////
//
template <class AType>
class FI_DLLEXPORT TautoPtr {
public:

  virtual ~TautoPtr() { Destroy(); }

  TautoPtr( AType*& p) { m_ptr = &p; }

  AType* operator->() const {
    return m_ptr ? *m_ptr : nullptr;
  }

  operator AType* () const {
    return m_ptr ? *m_ptr : nullptr;
  }

  TautoPtr& operator=( AType*& ptr) {
    m_ptr = &ptr;
    return *this;
  }

  AType*  Ptr()    { return m_ptr ? *m_ptr : nullptr; }
  void  Disown()  { m_ptr = nullptr; }
  void  Destroy()   { if ( m_ptr) { mc_del_ptr(*m_ptr);} }

private:

  DISALLOW_COPY_AND_ASSIGN(TautoPtr)
  AType**  m_ptr;
};

//////////////////////////////////////////////////////////////////////////////
//
#define mc_autoptrarr(Class, aPtr)   TautoPtrArr <Class> autoptrarr_##aPtr (aPtr)
#define mc_rel_autoptrarr(aPtr)   autoptrarr_##aPtr.Disown ()
template <class AType>
class FI_DLLEXPORT TautoPtrArr {
public:

  virtual ~TautoPtrArr() { Destroy(); }

  TautoPtrArr( AType*& p) { m_ptr = &p; }

  operator AType* () const {
    return m_ptr ? *m_ptr : nullptr;
  }

  TautoPtrArr& operator=( AType*& p) {
    m_ptr = &p;
    return *m_ptr;
  }

  AType*  Ptr()    { return m_ptr ? *m_ptr : nullptr; }
  void  Disown()  { m_ptr = nullptr; }
  void  Destroy()  { if ( m_ptr) { mc_del_arr(*m_ptr);} }

private:

  DISALLOW_COPY_AND_ASSIGN(TautoPtrArr)
  AType**  m_ptr;
};

#endif
