#include "node.h"

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
void Node::Add(const string& field, obj) {
  m_dict.setObject(obj, field);
}
