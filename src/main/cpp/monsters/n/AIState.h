
class AISystem;
class Entity;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AIState {
  DECL_TV(sstr, name, "?")
  virtual void enter() {}
  virtual void exit() {}
  virtual void updateEntity(not_null<Entity*>) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AIStateRush : public AIState {
  virtual void enter();
  virtual void update(not_null<Entity*>);
  AIStateRush() {
    name="Rush";
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AIStateMass : public AIState {
  virtual void enter();
  virtual void update(not_null<Entity*>);
  AIStateMass() {
    name="Mass";
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AIStateDefend : public AIState {

  virtual void enter();
  virtual void update(not_null<Entity*>);
  AIStateDefend() {
    name="Defend";
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AIStateCounter : public AIState {

  virtual void update(not_null<Entity*>);
  AIStateCounter() {
    name="Counter";
  }

};


NS_END



