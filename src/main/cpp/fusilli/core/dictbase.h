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

#if !defined(__DICTBASE_H__)
#define __DICTBASE_H__

#include "dicthlp.h"
NS_FI_BEGIN
//////////////////////////////////////////////////////////////////////////////
//
class FI_DLLEXPORT DictBase {
#define DICT_CLSPTR  0
#define DICT_ARRPTR  1
public:

  virtual ~DictBase();

  int   GetCount() const;
  bool  IsEmpty() const;

  void  RemoveAll(bool bDeepDelete = false);
  bool  RemoveKey(const std::string&);

protected:

  Tassoc**  m_hashTable;
  Tassoc*   m_freeList;
  Tassoc*   m_anchor;

  size_t     m_hashTableSize;
  struct Tpool*  m_blocks;
  int      m_ptrType;
  int     m_count;
  int     m_blockSize;

  DictBase( int = DICT_CLSPTR);

  bool   Lookup1(const std::string&, Tassoc*&) const;
  bool   Lookup0(const std::string& ) const;

  virtual void  DeleteElement( void*) = 0;
  Tassoc*  operator[](const std::string&);

  void   SetAt_(const std::string&, void*);
  void   SetAt_(const std::string&, long);
  void   SetAt_(const std::string&, double);

  void   GetNextAssoc_(void*&, std::string&, Tassoc*&) const;
  void*  GetStartPosition_() const;

  void   InitHashTable(size_t hashSize);
  size_t GetHashTableSize() const;
  size_t HashKey(const std::string& key) const;

  Tassoc*  GetAssocAt(const std::string&, size_t&, bool = true) const;
  Tassoc*  NewAssoc();
  void     FreeAssoc(Tassoc*);

  Tassoc*    GetAssocAt(const std::string&) const;
  Tassoc*    RemoveAssocAt(const std::string&);
};

inline int DictBase::GetCount() const {
  return m_count;
}

inline bool DictBase::IsEmpty() const {
  return (0 == m_count);
}

NS_FI_END
#endif
