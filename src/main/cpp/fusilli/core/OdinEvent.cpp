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

#include "base/ccUtils.h"
#include "OdinEvent.h"
NS_BEGIN(fusii)
NS_BEGIN(odin)

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::OdinEvent(MType t, EType c, j::json& body) : OdinEvent() {
    this->doco = j::json { {
    { "type", (int)t },
    { "code", (int)c }
    }};
  this->type= t;
  this->code= c;
  if (!body.is_null()) {
      auto p = this->doco["source"]; p = body;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::OdinEvent(MType t, EType c) : OdinEvent() {
  type= t;
  code= c;
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::OdinEvent(j::json& msg) : OdinEvent() {
  if (msg.is_object()) {
    auto v= msg["type"];
    if (v.is_number()) {
      type = SCAST(MType, v.get< j::json::number_integer_t >());
    }
    v= msg["code"];
    if (v.is_number()) {
      code = SCAST(EType, v.get<j::json::number_integer_t >());
    }
    v= msg["source"];
    if (!v.is_null()) {
      doco =v;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::OdinEvent() : c::EventCustom("odin") {
  tstamp = (double) c::utils::getTimeInMilliseconds();
  type= MType::NICHTS;
  code= EType::NICHTS;
  doco= j::json {};
}

//////////////////////////////////////////////////////////////////////////////
//
OdinEvent::~OdinEvent() {
}



NS_END(odin)
NS_END(fusii)

