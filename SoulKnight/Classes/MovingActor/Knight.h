#ifndef __KNIGHT_H__
#define __KNIGHT_H__
#include "cocos2d.h"
#include "Fighter.h"


USING_NS_CC;

class HelloWorld;

class Knight :public Fighter
{
	
	CC_SYNTHESIZE(bool,isRelease,IsRelease);
	CC_SYNTHESIZE(bool, canRelease, CanRelease);
	//Crit rate
public:

	virtual bool init(GameScene* Scene, std::string Name);

	static Knight* create(GameScene* Scene,std::string Name);

	virtual void releaseSkill();               //发起技能

	virtual void updateSkill();

	virtual bool attack();
};


#endif //!__KNIGHT_H__