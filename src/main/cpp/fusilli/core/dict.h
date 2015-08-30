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

#if !defined(__DICT_H__)
#define __DICT_H__

#include "dictbase.h"
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
template <class APtrType>
class FI_DLLEXPORT Dict : public DictBase {
public:

  Dict( int nType = DICT_CLSPTR)
    : DictBase(nType) {
  }

  virtual ~Dict()  {}

  bool  Lookup(const std::string&, APtrType& ) const;
  bool  Lookup(const std::string&) const;

  void  SetAt(const std::string&, APtrType);

  //= Iteration
  //
  void*  GetStartPosition() const;
  void GetNextAssoc(void*&, std::string&, APtrType&) const;

protected:

  virtual void  DeleteElement(void*);

private:

  DISALLOW_COPY_AND_ASSIGN(Dict)
};

//////////////////////////////////////////////////////////////////////////////
//
template <class APtrType>
void Dict<APtrType>::SetAt(const std::string& pzKey, APtrType pData) {
  SetAt_(pzKey, (void*)pData);
}

//////////////////////////////////////////////////////////////////////////////
//
template <class APtrType>
bool Dict<APtrType>::Lookup(const std::string& pzKey ) const {
  return Lookup0( pzKey);
}

//////////////////////////////////////////////////////////////////////////////
//
template <class APtrType>
bool Dict<APtrType>::Lookup(const std::string& pzKey, APtrType& pData) const {
  Tassoc*  pa;

  if ( Lookup1( pzKey, pa )) {
    pData = (APtrType) pa->m_value.pv;
    return true;
  } else {
    pData = nullptr;
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template <class APtrType>
void* Dict<APtrType>::GetStartPosition() const {
  return GetStartPosition_();
}

//////////////////////////////////////////////////////////////////////////////
//
template <class APtrType>
void Dict<APtrType>::GetNextAssoc(void*& nPos, std::string& pzKey, APtrType& pData) const {
  Tassoc*  pa;
  GetNextAssoc_(nPos, pzKey, pa);
  pData = (APtrType) pa->m_value.pv;
}

//////////////////////////////////////////////////////////////////////////////
//
template <class APtrType>
void Dict<APtrType>::DeleteElement(void* value) {
  switch ( m_ptrType) {
    case DICT_ARRPTR:  delete[] ((APtrType)value); break;
    case DICT_CLSPTR:  delete ((APtrType)value); break;
  }
}



NS_FI_END
#endif
