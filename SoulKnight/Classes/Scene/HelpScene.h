#pragma once
#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include "cocos2d.h"

class Help:public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	void menuExitCallBack(cocos2d::Ref* pSender);//返回上一场景

	CREATE_FUNC(Help);
};


#endif