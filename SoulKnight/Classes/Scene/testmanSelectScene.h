#pragma once
#ifndef __TESTMANSLECT_SCENE_H__
#define __TESTMANSLECT_SCENE_H__

#include "cocos2d.h"
#include"SafetyMapScene.h"
USING_NS_CC;
class testmanSelect :public cocos2d::Scene
{
	CC_SYNTHESIZE(SafetyMap*, _pSafetyMap, PSafetyMap);
	CC_SYNTHESIZE(int, _diamondNum, DiamondNum);
	CC_SYNTHESIZE(int, _grade, Grade);
	Label* diamond;
	Sprite* diamondPicture;
	Sprite* grade;
public:
	static cocos2d::Scene* createScene(SafetyMap* scene);
	static testmanSelect* create(SafetyMap* scene);
	virtual bool init(SafetyMap* scene);
	void initDiamond();
	void initUpgrade();
	void Upgrade();
	void menuUpgradeCallBack(cocos2d::Ref* pSender);
	void menuExitCallBack(cocos2d::Ref* pSender);//返回上一场景
	void menuPlayCallBack(cocos2d::Ref* pSender);//开始游戏
	void menuMenuCallBack(cocos2d::Ref* pSender);
	//CREATE_FUNC(testmanSelect);
};


#endif
