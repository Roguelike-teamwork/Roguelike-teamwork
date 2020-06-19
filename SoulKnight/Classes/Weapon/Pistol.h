#pragma once
#ifndef __PISTOL_H__
#define __PISTOL_H__

#include"cocos2d.h"
#include"MovingActor/Equipment.h"

USING_NS_CC;

class Pistol :public Equipment
{
public:

	virtual bool init(EAttackMode _equipType, String _weaponName, int _attack,
		float _attackSpeed, int _attackRadius, int _manaConsume
	);

	static Pistol* create(EAttackMode _equipType, String _weaponName, int _attack,
		float _attackSpeed, int _attackRadius, int _manaConsume
	);
};


#endif // !__PISTOL_H__
