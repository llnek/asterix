
NodePool::NodePool(nodeClass, components) {
  this.nodeClass = nodeClass;
  this.components = components;
}

NodePool::Get() {
  if( m_tail) {
    auto node = m_tail;
    m_tail = m_tail->previous;
    node->previous = nullptr;
    return node;
  } else {
    return new this.nodeClass();
  }
}

NodePool::Dispose(Node* node ) {
  this.components.forEach(function(componentClass, componentName) {
    node[componentName] = null;
  });
  node->entity = nullptr;
  node->next = nullptr;
  node->previous = m_tail;
  m_tail = node;
}

NodePool::Cache(Node* node ) {
  node->previous = this.cacheTail;
  this.cacheTail = node;
}

NodePool::ReleaseCache() {
  while ( this.cacheTail ) {
    auto node = this.cacheTail;
    this.cacheTail = node->previous;
    Dispose( node );
  }
}

