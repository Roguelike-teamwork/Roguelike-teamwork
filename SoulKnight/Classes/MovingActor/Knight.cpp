#include"Knight.h"
#include"Scene/GameScene.h"
#include"MovingActor/Equipment.h"
#include"MovingActor/Bullet.h"
#include"Weapon/Fork.h"
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
	//待初始化数据
	hitPoints = 1000000;
	lastSkillTime = 5.0f;
	skillCDTime = 10.0f;
	//TBD

	return true;
}


void Knight::releaseSkill()
{
	auto nowTime = GetCurrentTime()/1000;
	for (auto& it : myWeapon)
	{
		if (it->getEquipType() == MELEE)
			it->setAttackSpeedNumber(it->getAttackSpeedNumber() / 2);
	}
	isRelease = true;
	canRelease = false;
	lastReleaseTime = nowTime;
}

void Knight::updateSkill()
{
	auto nowTime = GetCurrentTime() / 1000;

	if (nowTime - lastReleaseTime >= skillCDTime)
		canRelease = true;

	if (nowTime - lastReleaseTime >= lastSkillTime&&isRelease)
	{
		isRelease = false;
		for (auto& it : myWeapon)
		{
			if (it->getEquipType() == MELEE)
				it->setAttackSpeedNumber(it->getAttackSpeedNumber() * 2);
		}
	}

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
		if (currentWeapon->getEquipType() == REMOTE)
		{
			for (int i = 0; i < fireTimes; i++)
			{
				auto bulletSprite = Bullet::create(CCString::createWithFormat("ArtDesigning/FlyingItem/Bullet/%sBullet.png", currentWeapon->getWeaponName().getCString())->getCString(),
					currentWeapon->getAttackNumber(),
					currentWeapon->getFlySpeed(),
					this,
					attackTarget);
				bulletSprite->giveOut();
				bulletSprite->setScale(0.75f, 0.75f);

				if (isRelease)
					bulletSprite->setPosition(Vec2(this->getPosition().x + (2 * i - 1) * 5 * sin(bulletSprite->getAngle() / 180 * M_PI),
						this->getPosition().y + (1 - 2 * i) * 5 * cos(bulletSprite->getAngle() / 180 * M_PI)));

				bulletSprite->setAttackMode(REMOTE);
				exploreScene->getMap()->addChild(bulletSprite);
				exploreScene->flyingItem.pushBack(bulletSprite);
			}
		}
		else
		{
			auto it = dynamic_cast<Fork*>(currentWeapon);
			for (int i = 0; i < 1; i++)
			{
				it->cut();
			}
		}
		lastAttackTime = GetCurrentTime()/1000.f;
		return true;
	}
	else if (!attackTarget)
	{
		if (currentWeapon->getEquipType() == REMOTE)
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
					bulletSprite->setPosition(Vec2(this->getPosition().x + (2 * i - 1) * 5 * sin(bulletSprite->getAngle() / 180 * M_PI),
						this->getPosition().y + (1 - 2 * i) * 5 * cos(bulletSprite->getAngle() / 180 * M_PI)));

				bulletSprite->setScale(0.75f, 0.75f);
				bulletSprite->setAttackMode(REMOTE);
				exploreScene->getMap()->addChild(bulletSprite);
				exploreScene->flyingItem.pushBack(bulletSprite);
			}
		}
		else
		{
			auto it = dynamic_cast<Fork*>(currentWeapon);
			for (int i = 0; i < 1; i++)
			{
				it->cut();
			}
		}
		lastAttackTime = GetCurrentTime()/1000.f;
		return true;
	}

	return false;
}


