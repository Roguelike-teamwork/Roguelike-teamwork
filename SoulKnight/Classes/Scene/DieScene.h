#pragma once
#ifndef __DIESCENE_H__
#define __DIESCENE_H__

#include "cocos2d.h"

USING_NS_CC;


class DieScene :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuRestartCallBack(cocos2d::Ref* pSender);
	void menuAudioCallBack(cocos2d::Ref* pSender);
	void menuExitCallBack(cocos2d::Ref* pSender);//·µ»Øsafetymap

	CREATE_FUNC(DieScene);
};
#endif