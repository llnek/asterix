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

#include "gnodes.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
owner<a::Node*> BoardNode::ReifyNode() {

  s::map<stdstr,a::COMType> s;

  s.insert(s::pair<stdstr,a::COMType>("selection", "n/UISelection"));
  s.insert(s::pair<stdstr,a::COMType>("board", "n/Board"));
  s.insert(s::pair<stdstr,a::COMType>("robot", "n/SmartAlgo"));
  s.insert(s::pair<stdstr,a::COMType>("grid", "n/Grid"));
  s.insert(s::pair<stdstr,a::COMType>("view", "n/GridView"));

  return a::Node::Reify(s);
};

//////////////////////////////////////////////////////////////////////////////
owner<a::Node*> GUINode::ReifyNode() {

  s::map<stdstr,a::COMType> s;

  s.insert(s::pair<stdstr,a::COMType>("selection", "n/UISelection"));
  s.insert(s::pair<stdstr,a::COMType>("view", "n/GridView"));

  return a::Node::Reify(s);
};

//////////////////////////////////////////////////////////////////////////////
owner<a::Node*> NetPlayNode::ReifyNode() {

  s::map<stdstr,a::COMType> s;

  s.insert(s::pair<stdstr,a::COMType>("playcmd", "n/NetPlay"));
  s.insert(s::pair<stdstr,a::COMType>("grid", "n/Grid"));

  return a::Node::Reify(s);
};




NS_END(tttoe)


