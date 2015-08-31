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

#include "dictbase.h"
NS_FI_BEGIN


//////////////////////////////////////////////////////////////////////////////
//
void Tpool::FreeDataChain(Tpool*& head) {
  Tpool*  p = head;
  Tpool*  pn;

  while (p != nullptr) {
    pn = p->m_next;
    mc_free_ptr(p->m_data);
    mc_free_ptr(p);
    p = pn;
  }

  head = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
Tpool* Tpool::Create(Tpool*& head, size_t nMax) {
  Tpool* p = (Tpool*) ::malloc(sizeof(Tpool));
  p->m_data = ::calloc(nMax, sizeof(Tassoc));
  p->m_next = head;
  head = p;
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
DictBase::~DictBase() {
  RemoveAll();
  mc_free_ptr(m_anchor);
}

//////////////////////////////////////////////////////////////////////////////
//
DictBase::DictBase(int ptr) {
  m_anchor = (Tassoc*) ::malloc(sizeof(Tassoc));
  m_hashTableSize   = 17;  // default size
  m_hashTable   = nullptr;
  m_ptrType    = ptr;
  m_count     = 0;
  m_freeList = nullptr;
  m_blocks = nullptr;
  m_blockSize   = 10;
}

//////////////////////////////////////////////////////////////////////////////
//
size_t DictBase::HashKey(const std::string& key) const {
  auto cs= key.c_str();
  size_t nHash = 0;
  while (*cs) {
    nHash = (nHash<<5) + nHash + *cs++;
  }
  return nHash;
}

//////////////////////////////////////////////////////////////////////////////
//
void DictBase::InitHashTable(size_t nHashSize) {
  m_hashTableSize = nHashSize;
  mc_free_ptr(m_hashTable);
  m_hashTable = (Tassoc**) ::calloc( nHashSize, sizeof(Tassoc*));
}

//////////////////////////////////////////////////////////////////////////////
//
void DictBase::RemoveAll(bool bDeepDelete) {
  if (m_hashTable != nullptr) {
    for (size_t nHash = 0; nHash < m_hashTableSize; ++nHash) {
      for (auto pa = m_hashTable[nHash]; pa; pa = pa->m_next) {
        if (bDeepDelete &&
            ASSOC_VOID == pa->m_dataType) {
          DeleteElement(pa->m_value.pv);
        }
      }
    }
    mc_free_ptr(m_hashTable);
  }
  Tpool::FreeDataChain(m_blocks);
  m_freeList = nullptr;
  m_count   = 0;
  m_blocks   = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
Tassoc* DictBase::NewAssoc() {
  Tassoc*  pa;

  if (m_freeList == nullptr) {
    // add another block
    // chain them into free list
    Tpool* newBlock = Tpool::Create(m_blocks, m_blockSize);
    pa = (Tassoc*) newBlock->Data();
    // free in reverse order to make it easier to debug
    pa += m_blockSize - 1;
    for (auto i = m_blockSize-1; i >= 0; --i, --pa) {
      pa->m_next = m_freeList;
      m_freeList = pa;
    }
  }

  pa  = m_freeList;
  m_freeList  = m_freeList->m_next;
  ++m_count;

  pa->m_dataType  = ASSOC_BAD;
  return pa;
}

//////////////////////////////////////////////////////////////////////////////
//
void DictBase::FreeAssoc(Tassoc* pa) {

  pa->m_next   = m_freeList;
  m_freeList   = pa;
  --m_count;

  if (IsEmpty()) {
    RemoveAll();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
Tassoc* DictBase::GetAssocAt(const std::string& key, size_t& nHash ) const {

  if ( m_hashTable == nullptr) { return nullptr; }

  nHash = HashKey(key) % m_hashTableSize;

  for (auto pa = m_hashTable[nHash]; pa; pa = pa->m_next) {
    if ( 0 == key.compare(pa->m_key)) {
      return pa;
    }
  }

  return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
Tassoc* DictBase::GetAssocAt(const std::string& key) const {
  if (m_hashTable && m_hashTableSize > 0) {
    for (size_t i = 0; i < m_hashTableSize; ++i) {
      for (auto pa = m_hashTable[i]; pa; pa = pa->m_next) {
        if (0 == key.compare(pa->m_key)) { return pa; }
      }
    }
  }
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
bool DictBase::Lookup0(const std::string& key ) const {
  Tassoc*  dummy;
  return Lookup1(key, dummy);
}

//////////////////////////////////////////////////////////////////////////////
//
bool DictBase::Lookup1(const std::string& key, Tassoc*& value) const {
  size_t   nHash;
  value = GetAssocAt(key, nHash );
  return (value != nullptr);
}

//////////////////////////////////////////////////////////////////////////////
//
Tassoc* DictBase::operator[](const std::string& key) {
  size_t nHash;
  auto pa= GetAssocAt(key, nHash);

  if (pa == nullptr) {
    if (m_hashTable == nullptr) {
      InitHashTable(m_hashTableSize);
    }
    // it doesn't exist, add a new Association
    pa = NewAssoc();
    pa->m_hash  = nHash;
    pa->m_key = key;
    // put into hash table
    pa->m_next = m_hashTable[nHash];
    m_hashTable[nHash]   = pa;
  }

  pa->m_dataType  = ASSOC_BAD;
  return pa;
}

//////////////////////////////////////////////////////////////////////////////
//
bool DictBase::RemoveKey(const std::string& key) {
  if ( m_hashTable == nullptr) { return false; }

  auto hk = HashKey(key) % m_hashTableSize;
  auto ppaPrev = &m_hashTable[hk];

  for (auto pa = *ppaPrev; pa; pa = pa->m_next) {
    if (0 == key.compare(pa->m_key)) {
      // remove it
      *ppaPrev = pa->m_next;  // remove from list
      FreeAssoc(pa);
      return true;
    }
    ppaPrev = &pa->m_next;
  }

  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
void DictBase::GetNextAssoc_(void*& pos, std::string& key, Tassoc*& value) const {
  auto paRet = (Tassoc*) pos;

  if (paRet == m_anchor) {
    for (size_t b = 0; b < m_hashTableSize; ++b) {
      if ((paRet = m_hashTable[b]) != nullptr) {
        break;
      }
    }
  }

  // find next association
  Tassoc* paNext;
  if ((paNext = paRet->m_next) == nullptr) {
    // go to next bucket
    for (auto b = paRet->m_hash + 1; b < m_hashTableSize; ++b) {
      if ((paNext = m_hashTable[b]) != nullptr) { break; }
    }
  }

  pos   = (void*) paNext;
  key   = paRet->m_key;
  value = paRet;
}

//////////////////////////////////////////////////////////////////////////////
//
void* DictBase::GetStartPosition_() const {
  return IsEmpty() ? nullptr : (void*) m_anchor;
}

//////////////////////////////////////////////////////////////////////////////
//
size_t DictBase::GetHashTableSize() const {
  return m_hashTableSize;
}

//////////////////////////////////////////////////////////////////////////////
//
void DictBase::SetAt_(const std::string& key, void* value) {
  auto pa = (*this)[key];
  pa->m_dataType   = ASSOC_VOID;
  pa->m_value.pv  = value;
}

//////////////////////////////////////////////////////////////////////////////
//
void DictBase::SetAt_(const std::string& key, long value) {
  auto pa = (*this)[key];
  pa->m_dataType   = ASSOC_LONG;
  pa->m_value.ln  = value;
}

//////////////////////////////////////////////////////////////////////////////
//
void DictBase::SetAt_(const std::string& key, double value) {
  auto pa = (*this)[key];
  pa->m_dataType   = ASSOC_REAL;
  pa->m_value.db  = value;
}

//////////////////////////////////////////////////////////////////////////////
//
Tassoc* DictBase::RemoveAssocAt(const std::string& key) {
  for (size_t i = 0; i < m_hashTableSize; ++i) {
    auto  ppaPrev = &m_hashTable[i];

    for (auto pa = *ppaPrev; pa; pa = pa->m_next) {
      if (0 == key.compare(pa->m_key)) {
        *ppaPrev = pa->m_next;
        return pa;
      }
      ppaPrev = & pa->m_next;
    }
  }
  return nullptr;
}





NS_FI_END

