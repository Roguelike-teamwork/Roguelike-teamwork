#pragma once
#ifndef __ENEMYREMOTE_H__
#define __ENEMYREMOTE_H__

#include"cocos2d.h"
#include"Enemy.h"
#include"Fighter.h"
#include"Constant.h"

USING_NS_CC;

class GameScene;

class EnemyRemote :public Enemy
{
	CC_SYNTHESIZE(float, lastAttackTime, LastAttackTime);




public:


	virtual bool attack();   //攻击，父类中存在定义,此处为远程攻击

	static EnemyRemote* create(GameScene* Scene, std::string enemyName);
	bool init(GameScene* Scene, std::string enemyName);
	bool initData(GameScene* Scene, std::string enemyName);

	//死亡等具体化定义父类基本完备



};






#endif // !__ENEMYREMOTE_H__
