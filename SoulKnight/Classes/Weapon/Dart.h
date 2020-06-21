#pragma once
#ifndef  __DART_H__
#define  __DART_H__

#include "cocos2d.h"
#include "MovingActor/Equipment.h"

USING_NS_CC;

class Dart :public Equipment
{
public:

	virtual bool init(EAttackMode _equipType, 
		String _weaponName, int _attack,
		float _attackSpeed, int _attackRadius, int _manaConsume
	);

	static Dart* create(EAttackMode _equipType,
		String _weaponName, int _attack,
		float _attackSpeed, int _attackRadius, int _manaConsume
	);




};



#endif // ! __DART_H__
