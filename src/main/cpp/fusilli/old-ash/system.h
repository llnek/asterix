#if !defined(__SYSTEM_H__)
#define __SYSTEM_H__


class CC_DLL System  {
        previous: null, /* System */
        next: null, /* System */
        priority: 0,

  virtual ~System();
  System();

  void AddToEngine(engine) {
  }

  void RemoveFromEngine(engine) {
  }

  void Update(float time) {
  }

  bool Is(type) {
      //return type.prototype.isPrototypeOf(this);
  }

};







#endif


