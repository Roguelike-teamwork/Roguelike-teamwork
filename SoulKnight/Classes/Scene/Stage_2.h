#pragma once
#pragma once
#ifndef __STAGE_2_H__
#define __STAGE_2_H__

#include"cocos2d.h"
#include"MovingActor/Constant.h"
#include"Scene/GameScene.h"

USING_NS_CC;

class Stage_2 :public GameScene
{
public:
	static Scene* createScene();

	static Stage_2* create();

	virtual bool init();

	virtual void generateEnemies();

	virtual void initMapLayer();

	virtual void generateNpc();
};



#endif // !__STAGE_1_H__