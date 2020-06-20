#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include"cocos2d.h"
#include"MovingActor/Constant.h"
#include"MovingActor/MovingActor.h"

USING_NS_CC;

class Enemy :public MovingActor
{
	CC_SYNTHESIZE(bool,isToMove, IsToMove)
	CC_SYNTHESIZE(bool,isMoving, IsMoving);
	CC_SYNTHESIZE(bool,everAttack,EverAttack);
	CC_SYNTHESIZE(String, enemyName, EnemyName);
	CC_SYNTHESIZE(Vec2, destination, Destination);
	CC_SYNTHESIZE(Vec2, oldDestination, OldDestination);
	CC_SYNTHESIZE(float, attackRadius, AttackRadius);
	CC_SYNTHESIZE(EAttackMode,attackMode,AttackMode);
	CC_SYNTHESIZE(Stage_State,enemyState,EnemyState);
	CC_SYNTHESIZE(EnemyLevel, level, Level);
	CC_SYNTHESIZE(MovingActor*, fromFighter, FromFighter);
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
