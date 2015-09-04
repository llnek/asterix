#if !defined(__ENGINE_H__)
#define __ENGINE_H__


class CC_DLL Engine {

private:

  //familyClass: ComponentMatchingFamily,
  EntityList* entityList;
  SystemList* systemList;
  Dictionary* families;
  bool updating;
  //updateComplete: new signals.Signal(),

public:

  virtual ~Engine();
  Engine();

  Array* GetEntities();
  Array* GetSystems();

  void AddEntity(Entity* );

  void RemoveEntity(Entity* );

  void RemoveAllEntities() ;

  void componentAdded(Entity*, componentClass);

  Array*  GetNodeList(Node* nodeObject);

  void ReleaseNodeList(Node* nodeObject );

  void AddSystem(System* );

  System* GetSystem(const string& type );

  void RemoveSystem (System* );

  void RemoveAllSystems();

  void Update(float time);

};





#endif


