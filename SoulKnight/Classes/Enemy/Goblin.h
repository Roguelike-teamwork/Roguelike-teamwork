#pragma once
#ifndef __GOBLIN_H__
#define __GOBLIN_H__

#include"cocos2d.h"
#include"MovingActor/Enemy.h"

USING_NS_CC;

class GameScene;

class Goblin :public Enemy
{
	CC_SYNTHESIZE(cocos2d::CCAction*, normal, Normal);
public:
	static Goblin* create(GameScene* Scene, std::string fighterName);
	bool init(GameScene* Scene, std::string fighterName);
	bool initData(GameScene* Scene, std::string fighterName);
	bool attack();
	bool loadAnimation();
};




#endif // !__GOBLIN_H__
