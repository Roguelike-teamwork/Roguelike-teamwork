#pragma once
#ifndef __ENEMYMELEE_H__
#define __ENEMYMELEE_H__

#include"cocos2d.h"
#include"Enemy.h"
#include"Fighter.h"
#include"Constant.h"

USING_NS_CC;

class GameScene;

class EnemyMelee :public Enemy
{
	CC_SYNTHESIZE(float, lastAttackTime, LastAttackTime);




public:


	virtual bool attack();   //攻击，父类中存在定义，此处为近战攻击，无子弹动画

	static EnemyMelee* create(GameScene* Scene, std::string enemyName);
	bool init(GameScene* Scene, std::string enemyName);
	bool initData(GameScene* Scene, std::string enemyName);

	//吃buff效果放着罢
	
	void bindSprite(CCSprite* sprite);


};






#endif // !__ENEMYMELEE_H__
