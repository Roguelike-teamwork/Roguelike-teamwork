#pragma once
#ifndef __UNMOVINGACTOR_H__
#define __UNMOVINGACTOR_H__
#include<iostream>
#include <string>
#include <time.h>
#include "cocos2d.h"
#include "Constant.h"
#include"Buff.h"
#include"MovingActor.h"
#include"Fighter.h"
#include"Equipment.h"
USING_NS_CC;

class GameScene;
enum TOTALNPC
{
	BloodBox,
	ManaBox,
	CoinBox,
	Businessman,
	Statue,
};


class UnMovingActor :public cocos2d::Sprite
{

	CC_SYNTHESIZE(float, touchRadius, TouchRadius);    //触发半径大小设定 用于角色是否在触发范围内
	CC_SYNTHESIZE(bool, isExploreBox, IsExploreBox);    //是否为通关后的宝箱
	CC_SYNTHESIZE(TOTALNPC, npcType, NPCType);            //npc名字
	CC_SYNTHESIZE(String,npcName,NPCName);
	CC_SYNTHESIZE(bool,isConfirm,IsConfirm);
	CC_SYNTHESIZE(bool, isOver, IsOver);
	CC_SYNTHESIZE(bool,isMeet,IsMeet);
	CC_SYNTHESIZE(int, goodsType, GoodsType);
	CC_SYNTHESIZE(GameScene*, _spawnScene, SpawnScene);

public:

	Label* putMessages;

	static UnMovingActor* create(const std::string& filename, GameScene* Scene, TOTALNPC NPC,float radius);

	virtual bool init(const std::string& filename, GameScene* Scene, TOTALNPC NPC, float radius);

	virtual bool isInRadius(Fighter * fighter);                 //判断角色是否在范围内

	virtual void effect(Fighter* customer);

	Equipment* equipBack();             //买武器用
};












#endif // !__UNMOVINGACTOR_H__
