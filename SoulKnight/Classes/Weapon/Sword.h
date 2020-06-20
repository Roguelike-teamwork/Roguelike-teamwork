#pragma once
#pragma once

#ifndef __SWORD_H__
#define __SWORD_H__

#include "cocos2d.h"
#include "MovingActor/Bullet.h"
#include "MovingActor/Equipment.h"


USING_NS_CC;

class Sword :public Equipment
{

public:

	virtual bool init(EAttackMode _equipType, String _weaponName, int _attack,
		float _attackSpeed, int _attackRadius, int _manaConsume
	);


	virtual bool init(ValueVector& data, EAttackMode _equipType, EEQUIPMENT _equipname);


	static Sword* create(EAttackMode _equipType, String _weaponName, int _attack,
		float _attackSpeed, int _attackRadius, int _manaConsume
	);

	virtual bool cut();

};



#endif //