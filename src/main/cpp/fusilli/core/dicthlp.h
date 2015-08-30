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

#if !defined(__DICTHLP_H__)
#define __DICTHLP_H__

#include "fusilli.h"
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
struct FI_DLLEXPORT Tassoc {
#define ASSOC_BAD  2104
#define ASSOC_VOID  2105
#define ASSOC_LONG  2106
#define ASSOC_REAL  2107

  std::string    m_key;
  int      m_dataType;
  Tassoc*    m_next;
  size_t    m_hash;
  union {
    void*    pv;
    long    ln;
    double    db;
  } m_value;

};

//////////////////////////////////////////////////////////////////////////////
//
struct FI_DLLEXPORT Tpool {
  static Tpool*   Create(Tpool*& head, size_t nMax);
  static void  FreeDataChain( Tpool*&);

  void*  Data()  { return m_data; }

  Tpool*     m_next;
  void*    m_data;
};


NS_FI_END
#endif
