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

#include "Node.h"
NS_BEGIN(Ash)



Node::Node() {
  entity = nullptr;
  previous= nullptr;
  next= nullptr;
}

Node::RemoveField(const string& field) {

  props->removeObject(field);
}
    /**
    * A simpler way to create a node.
    *
    * Example: creating a node for component classes Point &amp; energy:
    *
    * var PlayerNode = Ash.Node.create({
    *   point: Point,
    *   energy: Energy
    * });
    *
    * This is the simpler version from:
    *
    * var PlayerNode = Ash.Node.extend({
    *   point: null,
    *   energy: null,
    *
    *   types: {
    *     point: Point,
    *     energy: Energy
    *   }
    * });
    */
Node* Node::Add(const string& field, component) {
  types->setObject(component->ClassId(), field);
  props->setObject(component, field);
  return this;
}





NS_END(Ash)

