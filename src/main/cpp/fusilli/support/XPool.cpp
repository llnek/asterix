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

#include "XPool.h"
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
  /**
   * Pre-populate a bunch of objects in the pool.
   * @memberof module:zotohlab/asx/pool~XPool
   * @method preSet
   * @param {Function} ctor object constructor
   * @param {Number} count
   */
void XPool::PreSet(ctor, count) {
  for (auto n=0; n < count; ++n) {
    auto rc= ctor(this);
    if (NNP(rc)) {
      m_pool->addObject(rc);
    }
  }
}

  /**
   * Find an object by applying this filter.
   * @memberof module:zotohlab/asx/pool~XPool
   * @method select
   * @param {Function} filter
   * @return {Object} the selected one
   */
Entity* XPool::Select(filter) {
  for (auto it = m_pool->begin(); it != m_pool->end(); ++it) {
    auto rc = static_cast<Entity*>(*it);
    if ( filter(rc)) {
      return rc;
    }
  }
  return nullptr;
}

  /**
   * Get an object from the pool and set it's status to true.
   * @memberof module:zotohlab/asx/pool~XPool
   * @method getAndSet
   * @return {Object}
   */
Entity* XPool::GetAndSet() {
  auto rc= Get();
  if (NNP(rc)) {
    rc->status=true;
  }
  return rc;
}

  /**
   * Get an object from the pool.  More like a peek.
   * @memberof module:zotohlab/asx/pool~XPool
   * @method get
   * @return {Object}
   */
Entity* XPool::Get() {
  for (auto it = m_pool->begin(); it != m_pool->end(); ++it) {
    auto e= static_cast<Entity*>(*it);
    if (! e->status) { return e; }
  }
  return nullptr;
}

  /**
   * Get the count of active objects.
   * @memberof module:zotohlab/asx/pool~XPool
   * @method actives
   * @return {Number}
   */
int XPool::Actives() {
  auto c=0;
  for (auto it = m_pool->begin(); it != m_pool->end(); ++it) {
    if (static_cast<Entity*>(*it)->status) {
      ++c;
    }
  }
  return c;
}

int XPool::Size() { return m_pool->count(); }

  /**
   * Like map, but with no output.
   * @memberof module:zotohlab/asx/pool~XPool
   * @method iter
   * @param {Function} func
   * @param {Object} target if null, use the pool
   */
void XPool::Foreach(func, target) {
  if (target == nullptr) { target = this; }
  for (auto it = m_pool->begin(); it != m_poo->end(); ++it) {
    func.call(target, static_cast<Entity*>(*it));
  }
}

  /**
   * Hibernate (status off) all objects in the pool.
   * @memberof module:zotohlab/asx/pool~XPool
   * @method deflate
   */
void XPool::Reset() {
  for (auto it = m_pool->begin(); it != m_poo->end(); ++it) {
    static_cast<Entity*>(*it)->deflate();
  }
}


XPool::~XPool() {
  m_pool->release();
}

XPool::XPool() {
  m_pool = Array::create();
  m_pool->retain();
}


NS_FI_END
