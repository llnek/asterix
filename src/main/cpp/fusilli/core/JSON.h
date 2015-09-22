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

#if !defined(__JSON_H__)
#define __JSON_H__

#include "json/stringbuffer.h"
#include "json/document.h"
#include "json/writer.h"
#include "aeon/fusilli.h"

NS_ALIAS(js, rapidjson)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
namespace json {

  owner<js::Document*> Parse(const stdstr& s) {
    auto doc= new js::Document();
    doc->Parse(s.c_str());
    return doc;
  }

  const stdstr Stringify(not_null<js::Document*> doc) {
    js::StringBuffer buf;
    js::Writer<js::StringBuffer> w(buf);
    doc->Accept(w);
    return buf.GetString();
  }

};


NS_END(fusilli)
#endif


