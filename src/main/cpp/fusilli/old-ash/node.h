
class CC_DLL Node {
public:

  virtual ~Node();
  Node();

  void Add(const string& field, Ref*);

protected:
  entity;
  previous;
  next;

private:
};

