#pragma once
#ifndef __CROW_H__
#define __CROW_H__

#include"cocos2d.h"
#include"MovingActor/Enemy.h"

USING_NS_CC;

class GameScene;

class Crow :public Enemy
{
	CC_SYNTHESIZE(cocos2d::CCAction*, normal, Normal);
public:
	static Crow* create(GameScene* Scene, std::string fighterName);
	bool init(GameScene* Scene, std::string fighterName);
	bool initData(GameScene* Scene, std::string fighterName);
	bool attack();
	bool loadAnimation();
};
#endif