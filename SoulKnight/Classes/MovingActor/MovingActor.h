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
	CC_SYNTHESIZE(int,curHitPoints,CurHitPoints) //��ǰ����ֵ
	CC_SYNTHESIZE(AllCamp, camp, Camp);//��Ӫ
	CC_SYNTHESIZE(INT32, hitPoints, Hitpoints);   //����ֵ
	CC_SYNTHESIZE(INT32, damageAbility, DamageAbility);   //����˺�

	CC_SYNTHESIZE(float, flySpeed, FlySpeed);
	CC_SYNTHESIZE(float, attackSpeed, AttackSpeed); //�����ٶ�/����Ƶ��
	
	CC_SYNTHESIZE(float,lastAttackTime,LastAttackTime);
	CC_SYNTHESIZE(float, identityRadius, IdentityRadius); //ʶ��뾶���ڸ�֪��Χ
	CC_SYNTHESIZE(float, moveSpeed, MoveSpeed); //�ƶ��ٶ�

	CC_SYNTHESIZE(bool, alreadyDead, AlreadyDead); //�����ж�
	CC_SYNTHESIZE(GameScene*, exploreScene, ExploreScene);   //���볡��
	CC_SYNTHESIZE(MovingActor*,attackFrom, AttackFrom);//�˺���Դ
	CC_SYNTHESIZE(MovingActor*, attackTarget, AttackTarget);//����Ŀ��

protected:

	virtual void initData(GameScene* Scene);  //��ֹ�ն���

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
