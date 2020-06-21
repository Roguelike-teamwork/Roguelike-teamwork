#pragma once
#ifndef __SAFETYMAP_SCENE_H__
#define __SAFETYMAP_SCENE_H__

#include "cocos2d.h"
#include <map>

USING_NS_CC;
class SafetyMap :public cocos2d::Scene
{
	CC_SYNTHESIZE(cocos2d::TMXTiledMap*, _map, Map);
	CC_SYNTHESIZE(cocos2d::Layer*, _manlayer, Manlayer);
	CC_SYNTHESIZE(int, _diamondNum, DiamondNum);
	CC_SYNTHESIZE(int, _grade, Grade);
	cocos2d::TMXTiledMap* _tileMap;
	Label* diamond;
	Sprite* diamondPicture;
private:
	//初始化
	void initMapLayer();
	void initDiamond();//右上角的钻石
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void menuAudioCallBack(cocos2d::Ref* pSender);
	void menuTestManCallBack(cocos2d::Ref* pSender);
	void menuMenuCallBack(cocos2d::Ref* pSender);
	void BookMenuCallBack(cocos2d::Ref* pSender);
	CREATE_FUNC(SafetyMap);
};


#endif