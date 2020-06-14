#pragma once
#ifndef __DRAGON_H__
#define __DRAGON_H__

#include"MovingActor/Enemy.h"
#include"cocos2d.h"

USING_NS_CC;

class GameScene;

enum SkillType
{
	ONE,
	TWO,
	THREE,
	NOTHING
};

class Dragon :public Enemy
{
	CC_SYNTHESIZE(cocos2d::CCAction*,normal,Normal);

	CC_SYNTHESIZE(bool, isReleaseSkill_1, IsReleaseSkill_1);
	CC_SYNTHESIZE(bool, isReleaseSkill_2, IsReleaseSkill_2);
	CC_SYNTHESIZE(bool, isReleaseSkill_3, IsReleaseSkill_3);
	CC_SYNTHESIZE(float, eachTime, EachTime);
	CC_SYNTHESIZE(float,endTime,EndTime);
	CC_SYNTHESIZE(float, startTime, StartTime);
	CC_SYNTHESIZE(SkillType, lastSkill, LastSkill);

public:
	static Dragon* create(GameScene* Scene, std::string fighterName);
	bool init(GameScene* Scene, std::string fighterName);
	bool initData(GameScene* Scene, std::string fighterName);
	bool loadAnimation();

	virtual void chaosBullets();
	virtual void roundBullets();
	virtual void groundFlame();
	virtual void releaseSkill();
	virtual void updateState();

	//给可怜的龙放几个技能
};


#endif // !__DRAGON_H__
