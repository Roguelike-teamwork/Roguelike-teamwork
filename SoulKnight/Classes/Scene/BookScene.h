#pragma once
#ifndef __BOOK_SCENE_H__
#define __BOOK_SCENE_H__

#include "cocos2d.h"

class BookScene :public cocos2d::Scene
{

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCancelCallBack(cocos2d::Ref* pSender);

	CREATE_FUNC(BookScene);
};


#endif
