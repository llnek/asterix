#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:

	Sprite* ball;
	Sprite* paddle;
	Sprite* edgeSp;

	PhysicsWorld* m_world;

	void setPhyWorld(PhysicsWorld* world){ m_world = world; };

	void tick(float dt);

	virtual void onEnter();

	void onTouchMoved(Touch *touch, Event *event);
	bool onContactBegin(EventCustom* event, const PhysicsContact& contact);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
