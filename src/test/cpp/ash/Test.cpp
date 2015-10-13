// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include <stdio.h>
#include "ash/NodeRegistry.h"
#include "ash/Node.h"
#include "ash/Engine.h"
#include <map>
NS_ALIAS(a, ash)


class FS_DLL COMP1 : public a::Component {
public:
  virtual const a::COMType TypeId() { return "c1"; }
  int value;
  COMP1() { value = 1; }
};
class FS_DLL COMP2 : public a::Component {
public:
  virtual const a::COMType TypeId() { return "c2"; }
  int value;
  COMP2() { value = 2; }
};
class FS_DLL COMP3 : public a::Component {
public:
  virtual const a::COMType TypeId() { return "c3"; }
  int value;
  COMP3() { value = 3; }
};
class FS_DLL COMP4 : public a::Component {
public:
  virtual const a::COMType TypeId() { return "c4"; }
  int value;
  COMP4() { value = 4; }
};


class FS_DLL Fac1  : public a::NodeFactory  {
public:
  virtual const a::NodeType TypeId() { return "Fac1"; }
  virtual ash::Node* ReifyNode() {
    s::map<stdstr, a::COMType> sc;
    sc.insert(s::pair<stdstr,a::COMType>("f1", "c1"));
    sc.insert(s::pair<stdstr,a::COMType>("f2", "c2"));
    return a::Node::Reify(sc);
  }
};

class FS_DLL Fac2 : public a::NodeFactory {
public:
  virtual const a::NodeType TypeId() { return "Fac2"; }
  virtual a::Node* ReifyNode() {
    s::map<stdstr,a::COMType> s;
    s.insert(s::pair<stdstr,a::COMType>("f3", "c3"));
    s.insert(s::pair<stdstr,a::COMType>("f4", "c4"));
    return a::Node::Reify(s);
  }
};

class FS_DLL Sys2 : public a::System {
public:
  virtual const a::SystemType TypeId() { return "s1"; }
  Sys2() : a::System(3) {}
  virtual ~Sys2() {}
};


static void SetUp() {
  auto r = a::NodeRegistry::Self();
  auto eng = ash::Engine::Reify();
  auto f1 = new Fac1();
  auto f2 = new Fac2();
  r->Register(not_null<a::NodeFactory*>(f1));
  r->Register(not_null<a::NodeFactory*>(f2));

  auto ss= new Sys2();
  eng->RegoSystem(not_null<a::System*>(ss));

  a::Component* c;
  auto e1= eng->ReifyEntity("g1");
  c = new COMP1();
  e1->Rego(not_null<a::Component*>(c));
  c = new COMP2();
  e1->Rego(not_null<a::Component*>(c));

  auto e2= eng->ReifyEntity("g2");
  c = new COMP3();
  e2->Rego(not_null<a::Component*>(c));
  c = new COMP4();
  e2->Rego(not_null<a::Component*>(c));

  auto g1= eng->GetEntities("g1");
  auto g2= eng->GetEntities("g2");
  auto g3 = eng->GetEntities();

  printf("g1 size = %d\n", (int)g1.size());
  printf("g2 size = %d\n", (int)g2.size());
  printf("g3 size = %d\n", (int)g3.size());

  e1 = g1.front();
  auto ec1 = e1->GetAll();

  e2 = g2.front();
  auto ec2 = e2->GetAll();

  printf("ec1 size = %d\n", (int)ec1.size());
  printf("ec2 size = %d\n", (int)ec2.size());

  auto ns1 = eng->GetNodeList("Fac1");
  auto ns2 = eng->GetNodeList("Fac2");

  auto b1 = ns1->ContainsWithin(not_null<a::Entity*>(e1));
  auto b2 = ns1->ContainsWithin(not_null<a::Entity*>(e2));
  printf("ns1 has e1 = %s\n", b1 ? "true" : "false");
  printf("ns1 has e2 = %s\n", b2 ? "true" : "false");

  auto b3= ns2->ContainsWithin(not_null<a::Entity*>(e1));
  auto b4= ns2->ContainsWithin(not_null<a::Entity*>(e2));
  printf("ns2 has e1 = %s\n", b3 ? "true" : "false");
  printf("ns2 has e2 = %s\n", b4 ? "true" : "false");

  auto ccc= ns1->head->Get("f1");
  printf("f1 value = %d\n", static_cast<COMP1*>(ccc)->value);

  ccc= ns1->head->Get("f2");
  printf("f2 value = %d\n", static_cast<COMP2*>(ccc)->value);

  ccc= ns2->head->Get("f3");
  printf("f3 value = %d\n", static_cast<COMP3*>(ccc)->value);

  ccc= ns2->head->Get("f4");
  printf("f4 value = %d\n", static_cast<COMP4*>(ccc)->value);

  printf("ns1 size  = %d\n", ns1->Size());
  printf("ns2 size = %d\n", ns2->Size());

  // change e1, should alter ns1
  e1->Purge("c1");

  // add entity, should alter ns2
  auto e3= eng->ReifyEntity("g2");
  c = new COMP3();
  e3->Rego(not_null<a::Component*>(c));
  c = new COMP4();
  e3->Rego(not_null<a::Component*>(c));

  eng->Update(0);

  printf("AFTER UPDATE()!\n\n");

  ns1 = eng->GetNodeList("Fac1");
  ns2 = eng->GetNodeList("Fac2");

  printf("ns1 size  = %d\n", ns1->Size());
  printf("ns2 size = %d\n", ns2->Size());


  delete eng;
}


  int main(int ac, char** argv) {
    SetUp();
    printf("hello world\n");
    return 0;
  }





