#if !defined(__ENGINE_H__)
#define __ENGINE_H__


class CC_DLL Engine {

private:

  familyClass: ComponentMatchingFamily,
  Dictionary* m_families;
  EntityList*  m_ents;
  SystemList*  m_syss;
  bool m_updating;
  updateComplete;//: new signals.Signal()

public:

  virtual ~Engine();
  Engine();

  Array* GetEntities();
  Array* GetSystems();

  void AddEntity(Entity* );

  void RemoveEntity(Entity* );

  void RemoveAllEntities();

  void ComponentAdded(Entity* e, componentClass);

  Nodes GetNodeList(Node* nodeObject);

  void ReleaseNodeList(Node* nodeObject );

  void AddSystem(System* );

  System* GetSystem(const string& type);

  void RemoveSystem(System* );

  void RemoveAllSystems() ;
            while( this.systemList.head ) {
               this.removeSystem( this.systemList.head );
            }
        },

        update : function( time ) {
            this.updating = true;
            for( var system = this.systemList.head; system; system = system.next ) {
                system.update( time );
            }
            this.updating = false;
            this.updateComplete.dispatch();
        }
};




#endif

