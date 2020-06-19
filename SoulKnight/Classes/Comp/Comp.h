#pragma once
#ifndef __COMP_H__
#define __COMP_H__

#include"cocos2d.h"

USING_NS_CC;

class MovingActor;

class Comp:public Sprite
{
	CC_SYNTHESIZE(MovingActor*,owner,Owner);
	CC_SYNTHESIZE(ProgressTimer*, blood, Blood);
public:
	static Comp* create(MovingActor* sprite);
	virtual bool init(MovingActor* sprite);
	virtual void updateState();
};


#endif // !__COMP_H__
