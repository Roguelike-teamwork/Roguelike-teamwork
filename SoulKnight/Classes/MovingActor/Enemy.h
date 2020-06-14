#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include"cocos2d.h"
#include"MovingActor/MovingActor.h"

USING_NS_CC;

class Enemy :public MovingActor
{
	CC_SYNTHESIZE(bool,isToMove, IsToMove)
	CC_SYNTHESIZE(bool,isMoving, IsMoving);
	CC_SYNTHESIZE(String, enemyName, EnemyName);
	CC_SYNTHESIZE(Vec2, destination, Destination);
	CC_SYNTHESIZE(float, attackRadius, AttackRadius);
	CC_SYNTHESIZE(MovingActor*, fromFighter, FromFighter);
	CC_SYNTHESIZE(EDirection, fDirecition, FDirection);

	CCSprite* m_sprite;

public:
	static Enemy* create(GameScene* Scene, std::string fighterName);
	bool init(GameScene* Scene, std::string fighterName);
	bool initData(GameScene* Scene, std::string fighterName);
	bool attack();

	void bindSprite(CCSprite* sprite);
	void updateAction();
	void updateDestination();
	void updateTarget();
	void enemyMove();
	void die();
};


#endif // !__ENEMY_H__
