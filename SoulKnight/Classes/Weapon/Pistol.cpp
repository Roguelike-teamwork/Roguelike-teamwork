#include"Pistol.h"


Pistol* Pistol::create(EAttackMode _equipType, String _weaponName, int _attack,
	float _attackSpeed, int _attackRadius, int _manaConsume)
{
	Pistol* equipment = new Pistol();
	if (equipment && equipment->init(_equipType, _weaponName, _attack,
		_attackSpeed, _attackRadius, _manaConsume))
	{
		equipment->autorelease();

		return equipment;
	}
	CC_SAFE_DELETE(equipment);
	return nullptr;
}




bool Pistol::init(EAttackMode _equipType, String _weaponName, int _attack,
	float _attackSpeed, int _attackRadius, int _manaConsume)
{
	if (!Sprite::init())
	{
		return false;
	}

	equipType = _equipType;
	weaponName = _weaponName;

	attackNumber = _attack;
	attackSpeedNumber = _attackSpeed;
	attackRadius = _attackRadius;
	flySpeed = 12;

	manaConsume = _manaConsume;
	nowState = GROUND;

	this->setTexture("ArtDesigning/FlyingItem/Weapon/Pistol.png");
	return true;
}

bool Pistol::init(ValueVector& data, EAttackMode _equipType, EEQUIPMENT _equipname)
{
	if (!Sprite::init())
	{
		return false;
	}


	equipType = _equipType;
	equipName = _equipname;

	attackNumber = data.at(0).asInt();
	attackSpeedNumber = data.at(1).asFloat();
	attackRadius = data.at(2).asInt();
	manaConsume = data.at(3).asInt();

	
	return true;

}

