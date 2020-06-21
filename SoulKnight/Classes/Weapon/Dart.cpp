#include"Dart.h"


Dart* Dart::create(EAttackMode _equipType,
	String _weaponName, int _attack,
	float _attackSpeed, int _attackRadius, int _manaConsume)
{
	Dart* equipment = new Dart();

	if (equipment && equipment->init(_equipType, _weaponName, _attack,
		_attackSpeed, _attackRadius, _manaConsume))
	{
		equipment->autorelease();

		return equipment;


	}
	CC_SAFE_DELETE(equipment);

	return nullptr;


}



bool Dart::init(EAttackMode _equipType, String _weaponName,
	int _attack, float _attackSpeed, int _attackRadius, int _manaConsume)
{
	if (!Sprite::init())
	{
		return false;
	}
	equipType = _equipType;
	weaponName = _weaponName;
	equipName = DART;
	attackNumber = _attack;
	attackSpeedNumber = _attackSpeed;
	attackRadius = _attackRadius;
	flySpeed = DART_FLYSPEED;

	manaConsume = _manaConsume;
	nowState = GROUND;


	this->setTexture("ArtDesigning/FlyingItem/Weapon/Dart.png");
	return true;
}