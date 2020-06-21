#include"Sword.h"
#include"MovingActor/Bullet.h"
#include"Scene/GameScene.h"

Sword* Sword::create(EAttackMode _equipType, String _weaponName, int _attack,
	float _attackSpeed, int _attackRadius, int _manaConsume)
{
	Sword* equipment = new Sword();
	if (equipment && equipment->init(_equipType, _weaponName, _attack,
		_attackSpeed, _attackRadius, _manaConsume))
	{
		equipment->autorelease();

		return equipment;
	}
	CC_SAFE_DELETE(equipment);
	return nullptr;
}




bool Sword::init(EAttackMode _equipType, String _weaponName, int _attack,
	float _attackSpeed, int _attackRadius, int _manaConsume)
{
	if (!Sprite::init())
	{
		return false;
	}

	equipType = _equipType;
	weaponName = _weaponName;
	equipName = SWORD;
	attackNumber = _attack;
	attackSpeedNumber = _attackSpeed;
	attackRadius = _attackRadius;

	manaConsume = _manaConsume;

	this->setTexture("ArtDesigning/FlyingItem/Weapon/Sword.png");

	return true;
}

bool Sword::init(ValueVector& data, EAttackMode _equipType, EEQUIPMENT _equipname)
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

bool Sword::cut()
{
	//两种不同的方式计算角度
	float angle;
	if (owner->getAttackTarget())
	{
		auto victimVector = owner->getAttackTarget()->getPosition() - owner->getPosition();
		auto tan = victimVector.y / victimVector.x;

		if (tan > 0)
		{
			if (victimVector.y > 0)
				angle = atan(victimVector.y / victimVector.x) / M_PI * 180;
			else
				angle = atan(victimVector.y / victimVector.x) / M_PI * 180 + 180;
		}
		else
		{
			if (victimVector.x > 0)
				angle = atan(victimVector.y / victimVector.x) / M_PI * 180;
			else
				angle = atan(victimVector.y / victimVector.x) / M_PI * 180 + 180;
		}
	}
	else
	{
		int sita = 0;
		switch (owner->getLDriection())
		{
		case EDirection::RIGHT:
			sita = 0;
			break;
		case EDirection::UPRIGHT:
			sita = 1;
			break;
		case EDirection::UP:
			sita = 2;
			break;
		case EDirection::UPLEFT:
			sita = 3;
			break;
		case EDirection::LEFT:
			sita = 4;
			break;
		case EDirection::DOWNLEFT:
			sita = 5;
			break;
		case EDirection::DOWN:
			sita = 6;
			break;
		case EDirection::DOWNRIGHT:
			sita = 7;
			break;
		case EDirection::NODIR:
			sita = 2;
			break;
		default:
			break;
		}
		angle = sita * 180 / 4;
	}


	//生成攻击特效

	auto damage = Bullet::create("ArtDesigning/FlyingItem/Bullet/SwordEffect_1.png",
		attackNumber,
		0,
		owner,
		NULL);
	damage->setAttackMode(MELEE);
	damage->setTag(725);
	damage->setPosition(owner->getPosition().x + 20 * cos(angle / 180 * M_PI),
		owner->getPosition().y + 20 * sin(angle / 180 * M_PI));
	damage->setGiveOutTime(GetCurrentTime() / 1000.f);
	damage->setOwnerWeapon(this);
	if (!(angle >= 90 && angle <= 270))
		damage->setScaleX(-1.0);
	auto animation = Animation::create();
	animation = AnimationCache::getInstance()->getAnimation("Sword");
	animation->setDelayPerUnit(1.0f / 5.0f);
	animation->setLoops(-1);
	auto animate = Animate::create(animation);
	damage->runAction(RepeatForever::create(animate));

	owner->getExploreScene()->specialBullet.pushBack(damage);
	owner->getExploreScene()->getMap()->addChild(damage,6);

	return true;
}