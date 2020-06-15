#pragma once
#ifndef __MOVINGACTOR_H__
#define __MOVINGACTOR_H__
#include<iostream>
#include <string>
#include "cocos2d.h"
#include "MovingActor/Constant.h"

USING_NS_CC;

class GameScene;

class MovingActor :public cocos2d::Sprite
{
	CC_SYNTHESIZE(int, maxHitPoints, MaxHitpoints);   //生命值上限
	CC_SYNTHESIZE(int,curHitPoints,CurHitPoints) //当前生命值

	CC_SYNTHESIZE(AllCamp, camp, Camp);//阵营
	CC_SYNTHESIZE(INT32, hitPoints, Hitpoints);   //生命值

	CC_SYNTHESIZE(INT32, damageAbility, DamageAbility);   //造成伤害
	CC_SYNTHESIZE(float, flySpeed, FlySpeed);
	CC_SYNTHESIZE(float, identityRadius, IdentityRadius); //识别半径用于感知范围
	CC_SYNTHESIZE(float, moveSpeed, MoveSpeed); //移动速度
	CC_SYNTHESIZE(float, attackSpeed, AttackSpeed); //攻击速度/攻击频次
	CC_SYNTHESIZE(bool, alreadyDead, AlreadyDead); //死亡判定
	CC_SYNTHESIZE(GameScene*, exploreScene, ExploreScene);   //加入场景
	CC_SYNTHESIZE(MovingActor*,attackFrom, AttackFrom);//伤害来源
	CC_SYNTHESIZE(MovingActor*, attackTarget, AttackTarget);//攻击目标
	CC_SYNTHESIZE(cocos2d::CCSize, bodySize, BodySize);

protected:

	virtual void initData(GameScene* Scene);  //防止空定义

	//virtual void removeBuff(Buff* buff);
public:

	//
	virtual bool init(const std::string& filename, GameScene* Scene);
	virtual void die();
	virtual bool attack();
	virtual void takeDamage(DamageMode type, INT32 damage, MovingActor* enemy);
	static MovingActor* create(const std::string& filename,GameScene* Scene);
};





#endif // !__MOVINGACTOR_H__
