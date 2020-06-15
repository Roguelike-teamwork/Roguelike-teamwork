#include"Knight.h"
#include"Scene/GameScene.h"
#include"MovingActor/Equipment.h"
#include"MovingActor/Bullet.h"
//#include "HelloWorldScene.h"



Knight* Knight::create(GameScene* Scene, std::string Name)
{
	Knight* knight = new Knight();
	if (knight && knight->init(Scene, Name))
	{
		knight->autorelease();
		return knight;
	}

	CC_SAFE_DELETE(knight);
	return nullptr;
}



bool Knight::init(GameScene* Scene, std::string Name)
{
	if (!Fighter::init(Scene, Name))
	{
		return false;
	}
	//����ʼ������
	hitPoints = 1000000;
	//TBD

	return true;
}


void Knight::releaseSkill()
{
	auto nowTime = GetCurrentTime();
	isRelease = true;
	lastReleaseTime = nowTime;
}

void Knight::fullForce()
{
	auto nowTime = GetCurrentTime();
	if (nowTime - lastReleaseTime >= lastSkillTime&&isRelease)
		stopSkill();
}

void Knight::stopSkill()
{
	isRelease = false;
}

bool Knight::attack()
{
	if (!currentWeapon)
		return false;
	auto nowTime = GetCurrentTime()/1000.f;


	if (nowTime - lastAttackTime < currentWeapon->getAttackSpeedNumber())
		return false;


	int fireTimes;
	updateTarget();

	if (isRelease)
		fireTimes = 2;
	else
		fireTimes = 1;
		

	if (attackTarget)
	{
		for (int i = 0; i < fireTimes; i++)
		{
			auto bulletSprite = Bullet::create(CCString::createWithFormat("ArtDesigning/FlyingItem/Bullet/%sBullet.png", currentWeapon->getWeaponName().getCString())->getCString(),
											   currentWeapon->getAttackNumber(),
											   currentWeapon->getFlySpeed(),
											   this,
											   attackTarget);
			bulletSprite->giveOut();
			//bulletSprite->setScale();

			if (isRelease)
				bulletSprite->setPosition(Vec2(this->getPosition().x,this->getPosition().y+(2*i-1)*20));

			exploreScene->getMap()->addChild(bulletSprite);
			exploreScene->flyingItem.pushBack(bulletSprite);
		}
		lastAttackTime = GetCurrentTime()/1000.f;
		return true;
	}
	else if (!attackTarget)
	{
		for (int i = 0; i < fireTimes; i++)
		{
			auto bulletSprite = Bullet::create(CCString::createWithFormat("ArtDesigning/FlyingItem/Bullet/%sBullet.png", currentWeapon->getWeaponName().getCString())->getCString(),
				currentWeapon->getAttackNumber(),
				currentWeapon->getFlySpeed(),
				this,
				attackTarget);

			if (!isMoving)
				bulletSprite->giveOut(ldirection);
			else
				bulletSprite->giveOut(direction);

			if (isRelease)
				bulletSprite->setPosition(Vec2(this->getPosition().x, this->getPosition().y + (2 * i - 1) * 20));

			//bulletSprite->setScale();
			exploreScene->getMap()->addChild(bulletSprite);
			exploreScene->flyingItem.pushBack(bulletSprite);
		}
		lastAttackTime = GetCurrentTime()/1000.f;
		return true;
	}

	return false;
}


