#include"Fork.h"
#include"Scene/GameScene.h"

Fork* Fork::create(EAttackMode _equipType, EEQUIPMENT _equipname, int _attack,
	float _attackSpeed, int _attackRadius, int _manaConsume)
{
	Fork* equipment = new Fork();
	if (equipment && equipment->init(_equipType, _equipname, _attack,
		_attackSpeed, _attackRadius, _manaConsume))
	{
		equipment->autorelease();

		return equipment;
	}
	CC_SAFE_DELETE(equipment);
	return nullptr;
}




bool Fork::init(EAttackMode _equipType, EEQUIPMENT _equipname, int _attack,
	float _attackSpeed, int _attackRadius, int _manaConsume)
{
	if (!Sprite::init())
	{
		return false;
	}

	equipType = _equipType;
	equipName = _equipname;

	attackNumber = _attack;
	attackSpeedNumber = _attackSpeed;
	attackRadius = _attackRadius;

	manaConsume = _manaConsume;

	this->setTexture("ArtDesigning/FlyingItem/Weapon/Fork.png");
	this->setDamageRect(CCRectMake(0, 0, 20, 60));

	return true;
}

bool Fork::init(ValueVector& data, EAttackMode _equipType, EEQUIPMENT _equipname)
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

	this->setTexture("ArtDesigning\\FlyingItem\\Weapon\\Fork.png");
	this->setDamageRect(CCRectMake(0, 0, 20, 60));

	return true;

}

bool Fork::cut()
{
	float angle;
	if (onwer->getAttackTarget())
	{
		auto victimVector = onwer->getAttackTarget()->getPosition() - onwer->getPosition();
		angle = acos(victimVector.y / victimVector.getLength()) / M_PI * 180;
	}
	else {
		int sita = 0;
		switch (onwer->getLDriection())
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
	//设置判定区域
	onwerRect.origin = Vec2(onwer->getPosition().x - onwer->getBodySize().width / 2, onwer->getPosition().y - onwer->getBodySize().height);
	onwerRect.size.width = onwer->getBodySize().width;
	onwerRect.size.height = onwer->getBodySize().height;

	damageRect.origin = Vec2(onwer->getPosition().x + 40 * cos(angle), onwer->getPosition().y + 40 * sin(angle));
	

	//生成攻击特效
	auto flash = Sprite::create("ArtDesigning/FlyingItem/Bullet/ForkEffect.png");
	flash->setAnchorPoint(damageRect.origin);
	flash->setRotation(angle);
	auto action = Sequence::createWithTwoActions(DelayTime::create(0.5f),
												 Hide::create());
	flash->runAction(action);
	onwer->getExploreScene()->addChild(flash);

	return true;
}