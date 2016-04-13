// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "base/ccUtils.h"
#include "OdinEvent.h"
NS_BEGIN(fusii)
NS_BEGIN(odin)

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::OdinEvent(MType t, EType c, const j::json &body)
  : OdinEvent() {

  this->_doco = j::json( {
    { "type", (int)t },
    { "code", (int)c }
  });
  this->_type= t;
  this->_code= c;
  if (!body.is_null()) {
    this->_doco["source"] = body;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::OdinEvent(MType t, EType c) : OdinEvent() {
  _type= t;
  _code= c;
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::OdinEvent(const j::json &full_msg)
  : OdinEvent() {

  if (full_msg.is_object()) {
    auto v= full_msg["type"];
    if (v.is_number()) {
      _type = SCAST(MType, JS_INT(v));
    }
    v= full_msg["code"];
    if (v.is_number()) {
      _code = SCAST(EType, JS_INT(v));
    }
    v= full_msg["source"];
    if (v.is_object()) {
      _doco =v;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::OdinEvent() : c::EventCustom("odin") {
  _tstamp = c::utils::getTimeInMilliseconds();
  _doco= j::json::object_t();
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::~OdinEvent() {
  CCLOG("OdinEvent::dtor() called");
}


NS_END
NS_END



