#pragma once
#ifndef __BUFF_H__
#define __BUFF_H__
#include "cocos2d.h"
USING_NS_CC;

enum EBuffType
{
	VERTIGO,	//眩晕
	FROZEN,     //冰冻
	BURN,       //灼烧
	POISON,     //中毒
	SPEEDUP,    //加速
	SPEEDDOWN,  //减速
	NORMAL		//常规,也就是血瓶和魔瓶
};
//捡药瓶可归于buff类中
class Buff :public cocos2d::Sprite
{
	//CC_SYNTHESIZE(int, hp, HP);                          
	//CC_SYNTHESIZE(float, buffMoveSpeed, BuffMoveSpeed);

	CC_SYNTHESIZE(EBuffType,buffType,BuffType);			//buff的类型
	CC_SYNTHESIZE(int, buffHp, BuffHp);                   //血瓶效果,烧伤，中毒
	CC_SYNTHESIZE(int, buffMp, BuffMp);					 //魔瓶效果       能量药瓶效果
	CC_SYNTHESIZE(float, buffMoveSpeed, BuffMoveSpeed);   //移动速度效果   增益/减益 ,冰冻，眩晕
	

	CC_SYNTHESIZE(float, duration, Duration);   //持续时间
	CC_SYNTHESIZE(float, beginTime, BeginTime); //开始时间
	CC_SYNTHESIZE(float, endTime, EndTime);		//结束时间
public:
	bool init(EBuffType _bufftype,
		int _buffHP, int _buffMP,
		float _buffMoveSpeed, float _duration);
	
	static Buff* create(EBuffType _bufftype,
		int _buffHP, int _buffMP,
		float _buffMoveSpeed, float _duration);
};




#endif // !__BUFF_H__
