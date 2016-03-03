

#include "AIState.h"

NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AIStateRush : public AIState {
  DECL_TV(sstr, name, "?")
  virtual void enter() {}
  virtual void exit() {}
  virtual void updateEntity(not_null<Entity*>) {}
};




NS_END


