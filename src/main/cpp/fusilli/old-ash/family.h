#if !defined(__FAMILY_H__)
#define __FAMILY_H__


class CC_DLL Family {
public:

  virtual void newEntity(entity);

  virtual void removeEntity(entity) ;{

  virtual void componentAddedToEntity(entity, componentClass);

  virtual void componentRemovedFromEntity(entity, componentClass);

  virtual void cleanUp();

  virtual void nodeList() { return nodes; }

protected:

  Family(nodeObject, engine);

  nodes;

private:

};



#endif


