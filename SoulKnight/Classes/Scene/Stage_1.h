#pragma once
#ifndef __STAGE_1_H__
#define __STAGE_1_H__

#include"cocos2d.h"
#include"MovingActor/Constant.h"
#include"Scene/GameScene.h"

USING_NS_CC;

class testmanSelect;

class Stage_1 :public GameScene
{
public:
	static Scene* createScene(testmanSelect* scene);

	static Stage_1* create(testmanSelect* scene);

	virtual bool init(testmanSelect* scene);

	virtual void generateEnemies();

	virtual void initMapLayer();

	virtual void generateNpc();
};



#endif // !__STAGE_1_H__
;