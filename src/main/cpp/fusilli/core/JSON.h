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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "nlohmann/json.hpp"
#include "aeon/fusilli.h"
NS_ALIAS(j, nlohmann)

//////////////////////////////////////////////////////////////////////////////
//
#define J__LOOP(it,c) \
    for (j::json::iterator it= c.begin(); it != c.end(); ++it)

#define JS_FLOAT(obj) obj.get<j::json::number_float_t>()
#define JS_INT(obj) obj.get<j::json::number_integer_t>()
#define JS_STR(obj) obj.get<j::json::string_t>()
#define JS_BOOL(obj) obj.get<j::json::boolean_t>()
#define JS_ARR(obj) obj.get<j::json::array_t>()
#define JS_OBJ(obj) obj.get<j::json::object_t>()





