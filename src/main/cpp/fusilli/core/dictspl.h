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

#if !defined(__DICTSPL_H__)
#define __DICTSPL_H__

#include "dictbase.h"
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
class FI_DLLEXPORT MapLong : public DictBase {
public:

  virtual ~MapLong()  {}
  MapLong()    {}

  bool  Lookup(const std::string&, long& ) const;
  bool  Lookup(const std::string& ) const;
  void  SetAt(const std::string&, long);

  void  GetNextAssoc(void*&, std::string&, long&) const;
  void*  GetStartPosition() const;

protected:

  virtual  void  DeleteElement(void*);

private:

  DISALLOW_COPY_AND_ASSIGN(MapLong)
};

//////////////////////////////////////////////////////////////////////////////
//
inline void MapLong::SetAt(const std::string& pzKey, long ln) {
  SetAt_(pzKey, ln);
}

//////////////////////////////////////////////////////////////////////////////
//
inline bool MapLong::Lookup(const std::string& pzKey ) const {
  return Lookup0( pzKey);
}

//////////////////////////////////////////////////////////////////////////////
//
inline bool MapLong::Lookup(const std::string& pzKey, long& ln ) const {
  Tassoc*  pa;

  if ( Lookup1( pzKey, pa )) {
    ln = pa->m_value.ln;
    return true;
  } else {
    ln = 0;
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
inline void* MapLong::GetStartPosition() const {
  return GetStartPosition_();
}

//////////////////////////////////////////////////////////////////////////////
//
inline void MapLong::GetNextAssoc(void*& nPos, std::string& pzKey, long& ln) const {
  Tassoc*  pa;
  GetNextAssoc_(nPos, pzKey, pa);
  ln = pa->m_value.ln;
}

//////////////////////////////////////////////////////////////////////////////
//
inline void MapLong::DeleteElement(void*) {
}

//////////////////////////////////////////////////////////////////////////////
//
class FI_DLLEXPORT MapReal : public DictBase {
public:

  virtual ~MapReal()  {}
  MapReal()    {}

  bool  Lookup(const std::string&, double& ) const;
  bool  Lookup(const std::string& ) const;
  void  SetAt(const std::string&, double);

  void  GetNextAssoc( void*&, std::string&, double&) const;
  void*  GetStartPosition() const;

protected:

  virtual  void  DeleteElement(void*);

private:

  DISALLOW_COPY_AND_ASSIGN(MapReal)
};

//////////////////////////////////////////////////////////////////////////////
//
inline void MapReal::SetAt(const std::string& pzKey, double db) {
  SetAt_(pzKey, db);
}

//////////////////////////////////////////////////////////////////////////////
//
inline bool MapReal::Lookup(const std::string& pzKey ) const {
  return Lookup0( pzKey);
}

//////////////////////////////////////////////////////////////////////////////
//
inline bool MapReal::Lookup(const std::string& pzKey, double& db ) const {
  Tassoc*  pa;

  if ( Lookup1( pzKey, pa)) {
    db = pa->m_value.db;
    return true;
  } else {
    db = 0.0;
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
inline void* MapReal::GetStartPosition() const {
  return GetStartPosition_();
}

//////////////////////////////////////////////////////////////////////////////
//
inline void MapReal::GetNextAssoc( void*& nPos, std::string& pzKey, double& db) const {
  Tassoc*  pa;
  GetNextAssoc_(nPos, pzKey, pa);
  db = pa->m_value.db;
}

//////////////////////////////////////////////////////////////////////////////
//
inline void MapReal::DeleteElement( void*) {
}

NS_FI_END
#endif
