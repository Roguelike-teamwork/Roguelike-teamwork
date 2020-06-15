#pragma once
#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__
#include<iostream>
#include"cocos2d.h"
//#include"MovingActor/Fighter.h"
#include"Constant.h"

USING_NS_CC;

class Fighter;

class Equipment :public cocos2d::Sprite
{
public:

	CC_SYNTHESIZE(Fighter*, onwer, Onwer);
	CC_SYNTHESIZE(EAttackMode, equipType, EquipType);    //定义武器类型，远程or近战
	CC_SYNTHESIZE(EEQUIPMENT, equipName, EquipName);	   //武器名字
	
	CC_SYNTHESIZE(String,weaponName,WeaponName);									//名字
	CC_SYNTHESIZE(int, attackNumber, AttackNumber);					 //攻击力数值
	CC_SYNTHESIZE(float, attackSpeedNumber, AttackSpeedNumber);		//攻击速度数值
	CC_SYNTHESIZE(float, flySpeed, FlySpeed);						//其子弹的飞行速度
	CC_SYNTHESIZE(WeaponStatus, status, Status);					//武器的位置，人身上或地上

	//如果是近战武器，则应有
	CC_SYNTHESIZE(int, attackRadius, AttackRadius);               //攻击半径
	//如果是远程武器，则应有（元气貌似无最远距离）触碰消失/反弹
	CC_SYNTHESIZE(int, manaConsume, ManaConsume);         //法力值消耗

	//TBD
	//射击动画

	//EQUIPMENT初始化参数比较多，为了不至于把传参与变量弄混，加下划线做区分
	virtual bool init(EAttackMode _equipType, EEQUIPMENT _equipname, int _attack,
		float _attackSpeed, int _attackRadius,int _manaConsume
	);


	virtual bool init(ValueVector& data, EAttackMode _equipType, EEQUIPMENT _equipname);   //以plist文件直接进行定义


	static Equipment* create(EAttackMode _equipType, EEQUIPMENT _equipname, int _attack,
		float _attackSpeed, int _attackRadius, int _manaConsume
	);


};






#endif // !__EQUIPMENT_H__
