
#include "System.h"

class Entity;
class AIState;

struct CC_DLL AISystem : public f::System {

  DECL_FZ(humanQuirkValue)
  DECL_FZ(humanZapValue)
  DECL_FZ(humanMunchValue)
  DECL_FZ(humanTotalValue)
  DECL_FZ(aiQuirkValue)
  DECL_FZ(aiZapValue)
  DECL_FZ(aiMunchValue)
  DECL_FZ(aiTotalValue)

- (void)changeStateForEntity:(Entity *)entity toState:(AIState *)state;
- (void)spawnQuirkForEntity:(Entity *)entity;
- (void)spawnZapForEntity:(Entity *)entity;
- (void)spawnMunchForEntity:(Entity *)entity;

@end
