#pragma once
#ifndef __FIGHTER_H__
#define __FIGHTER_H__
#include "cocos2d.h"
#include "MovingActor/MovingActor.h"
#include "MovingActor/Constant.h"
#include "MovingActor/Enemy.h"
#include "MovingActor/Equipment.h"
#include "MovingActor/Buff.h"

USING_NS_CC;



class Fighter :public MovingActor
{
	CC_SYNTHESIZE(EDirection, direction, Direction);//人物的移动方向
	CC_SYNTHESIZE(EDirection, olddirection, OldDirection);//人物的面向  之后可以删掉了
	CC_SYNTHESIZE(EDirection, ldirection, LDriection);//
	CC_SYNTHESIZE(bool, isMoving, IsMoving);
	CC_SYNTHESIZE(bool, canAttack, CanAttack);
	CC_SYNTHESIZE(bool, isPlay, IsPlay);
	CC_SYNTHESIZE(bool, canBeHurt, CanBeHurt);

	CC_SYNTHESIZE(int, shield, Shield);                //护甲值上限
	CC_SYNTHESIZE(int, curShield, CurShield);		   //当前护甲值
	CC_SYNTHESIZE(float, acRcoverSpeed, ACRcoverSpeed); //护甲刷新时间
	CC_SYNTHESIZE(float, lastTimeInjured, LastTimeInjured); //最后受到伤害的时间，用于刷新护盾
	CC_SYNTHESIZE(float, lastTimeRecover, LastTimeRecover);

	CC_SYNTHESIZE(int, manaPoints, ManaPoints);			//法力值上限
	CC_SYNTHESIZE(int, curManaPoints, CurManaPoints);	//当前法力值

	CC_SYNTHESIZE(String, fighterName, FighterName);	//获取角色名
	CC_SYNTHESIZE(int, equipNumber, EquipNumber);     //所能携带的武器数量
	CC_SYNTHESIZE(Equipment*, currentWeapon, CurrentWeapon);  //当前武器

	//5.27从子类移至父类
	CC_SYNTHESIZE(float, lastAttackTime, LastAttackTime);    
	CC_SYNTHESIZE(float,lastReleaseTime,LastReleaseTime);
	CC_SYNTHESIZE(float, lastSkillTime, LastSkillTime);      //技能持续时间
	CC_SYNTHESIZE(float, skillCDTime, SkillCDTime);			//技能冷却时间
	CC_SYNTHESIZE(EBuffType, state, State);

	
	CCSprite* m_sprite;
public:

	Vector<Equipment*> myWeapon;
	
	Vector<Buff*> myBuff;

	virtual void loadAnimation();

	virtual bool isFullEquipments();               //判断是否带满武器

	virtual void updateCondition();

	virtual void getWeapon(Equipment* available);  //获得武器

	Equipment* changeMainEquip();  //切换主武器

	virtual bool attack();						  //是否攻击

	//virtual void takeBuff(Buff* buff);			//获得buff  TBD

	virtual void releaseSkill();               //发起技能

	virtual Vec2 updateDestination();

	virtual void fighterMove(Vec2 newPosition);               //发起移动

	virtual void stand();                     //停止移动后英雄的面向

	virtual bool isInMelee();                  //是否在近战范围

	virtual bool init(GameScene* Scene, std::string fighterName);

	static Fighter* create(GameScene* Scene, std::string fighterName);   //Unknown

	virtual void playAnimation();   //？？？ Unknown

	virtual bool isZeroSheild();           //判定护甲值是否为0
	
	virtual void updateTarget();            //刷新攻击目标

	virtual void hurt(INT32 damage);                    //受伤

	virtual void takeBuff(Buff* buff);

	virtual void removeBuff();


	virtual void bindSprite(CCSprite* sprite);

protected:


	ValueMap initFighterData;

	virtual void die();                  //角色死亡时进行的操作

	//virtual void removeAllBuff();			

	//virtual void removeBuff(Buff* buff);

	//virtual void updateDirection();
	//※※此处参照学长所做修正方向的函数，如有需要待定义，待编写

	virtual bool initHeroData(GameScene* Scene, std::string fighterName);
};



#endif // !__FIGHTER_H__
