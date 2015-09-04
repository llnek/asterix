#if !defined(__ENGINE_H__)
#define __ENGINE_H__

class CC_DLL Engine {

        //familyClass: ComponentMatchingFamily,
        //families: null,
        //entityList: null,
        //systemList: null,
        //updating: false,
        //updateComplete: new signals.Signal(),

Engine::Engine() {
  this.entityList = new EntityList(),
  this.systemList = new SystemList();
  this.families = new Dictionary();
}

Engine::GetEntities() {
  return m_ents;
}

Engine::GetSystems() {
  return m_syss;
}

Engine::AddEntity(Entity* e) {
  m_ents.Add(e);
  //entity.componentAdded.add( this.componentAdded, this );
  this.families.forEach( function( nodeObject, family ) {
      family.newEntity( entity );
  });
},

Engine::RemoveEntity(Entity* e) {
  //entity.componentAdded.remove( this.componentAdded, this );
  this.families.forEach( function( nodeObject, family ) {
      family.removeEntity( entity );
  });
  m_ents->Remove(e);
}

Engine::RemoveAllEntities() {
  m_ents->Clear();
}

Engine::ComponentAdded(entity, componentClass) {
    this.families.forEach( function( nodeObject, family ) {
        family.componentAddedToEntity( entity, componentClass );
    });
}

Engine::GetNodeList(nodeObject) {
  if( this.families.has( nodeObject ) ) {
      return this.families.retrieve( nodeObject ).nodes;
  }
  var family = new this.familyClass( nodeObject, this );
  this.families.add( nodeObject, family );
  for( var entity = this.entityList.head; entity; entity = entity.next ) {
      family.newEntity( entity );
  }
  return family.nodes;
}

Engine::ReleaseNodeList( nodeObject ) {
    if( this.families.has( nodeObject ) ) {
        this.families.retrieve( nodeObject ).cleanUp();
    }
    this.families.remove( nodeObject );
}

Engine::AddSystem(System* s) {
  s->AddToEngine( this );
  m_syss->Add(s);
}

Engine::GetSystem( type ) {
  return this.systemList.get( type );
}

Engine::RemoveSystem(System* s) {
  m_syss->Remove(s);
  s->RemoveFromEngine( this );
}

Engine::RemoveAllSystems() {
  while( this.systemList.head ) {
     this.removeSystem( this.systemList.head );
  }
}

Engine::Update(float time) {
  m_updating = true;
  for( var system = this.systemList.head; system; system = system.next ) {
      system.update( time );
  }
  m_updating = false;
  //this.updateComplete.dispatch();
}


#endif


