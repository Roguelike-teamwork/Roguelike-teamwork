#pragma once
#ifndef __RABBIT_H__
#define __RABBIT_H__

#include"cocos2d.h"
#include"MovingActor/Enemy.h"

USING_NS_CC;

class GameScene;

class Rabbit :public Enemy
{
	CC_SYNTHESIZE(cocos2d::CCAction*, normal, Normal);
public:
	static Rabbit* create(GameScene* Scene, std::string fighterName);
	bool init(GameScene* Scene, std::string fighterName);
	bool initData(GameScene* Scene, std::string fighterName);
	bool attack();
	bool loadAnimation();
};




#endif // !__GOBLIN_H__