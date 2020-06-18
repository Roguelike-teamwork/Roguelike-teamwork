#include"MovingActor/MovingActor.h"
#include"MovingActor/Fighter.h"
#include"MovingActor/Constant.h"
#include"MovingActor/Bullet.h"
#include"MovingActor/Equipment.h"
#include"Scene/GameScene.h"

#include<set>


Fighter* Fighter::create(GameScene* Scene ,std::string fighterName)
{

	Fighter* fighter = new Fighter();
	if (fighter && fighter->init(Scene, fighterName))
	{

		fighter->autorelease();
		return fighter;					//	？？？？波浪线原因未知
	}

	CC_SAFE_DELETE(fighter);
	return nullptr;


}




bool Fighter::init(GameScene* Scene, std::string fighterName)
{
	if (!Sprite::init())
	{
		return false;
	}

	initHeroData( Scene, fighterName);
	//其他初始定义待补充

	return true;

}


bool Fighter::initHeroData(GameScene* Scene, std::string Name)
{
	ValueMap value = FileUtils::getInstance()->getValueMapFromFile("FightersData.plist");
	//initFighterData = value.at(Name).asValueMap();

	exploreScene = Scene;
	fighterName = Name;
	camp = AllCamp::FRIENDLY;

	identityRadius = INIT_ID_RADIUS;
 
	equipNumber = INIT_EQUIP_NUMBER;

	canBeHurt = true;
	alreadyDead = false;
	attackSpeed = 0.f;
	attackMode = MIX;
	attackTarget = NULL;
	lastTimeInjured = 0.f;
	isMoving = false;
	//lastSkillTime = 0.f;
	setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sDown.png", fighterName.getCString())->getCString());


	curHitPoints = hitPoints;         //初始设定为满值
	curShield = shield;		
	curManaPoints = manaPoints;
			




	return true;
}

bool Fighter::isFullEquipments()
{
	if (myWeapon.size() == 2)
		return true;
	return false;
}

Equipment* Fighter::changeMainEquip()    //待添加切换武器的音效
{
	if (isFullEquipments())
	{
		for (auto it = myWeapon.begin(); it != myWeapon.end(); it++)
		{
			if (currentWeapon != *it)
				return *it;
		}
	}
	else
		return currentWeapon;
}

bool Fighter::attack()
{
	if (!currentWeapon)
		return false;

	updateTarget();

	if (attackTarget)
	{
		auto bulletSprite = Bullet::create( CCString::createWithFormat("%sBullet", currentWeapon->getWeaponName())->getCString(), 
											currentWeapon->getAttackNumber(), 
											currentWeapon->getFlySpeed(),
											this, 
											attackTarget);
		bulletSprite->giveOut();
		//bulletSprite->setScale();
		exploreScene->getMap()->addChild(bulletSprite);
		exploreScene->flyingItem.pushBack(bulletSprite);
		return true;
	}
	else if(!attackTarget)
	{
		auto bulletSprite = Bullet::create( CCString::createWithFormat("%sBullet", currentWeapon->getWeaponName())->getCString(),
											currentWeapon->getAttackNumber(),
											currentWeapon->getFlySpeed(),
											this,
											attackTarget);

		if (!isMoving)
			bulletSprite->giveOut(ldirection);
		else
			bulletSprite->giveOut(direction);
		//bulletSprite->setScale();
		exploreScene->addChild(bulletSprite);
		exploreScene->flyingItem.pushBack(bulletSprite);
		return true;
	}

	return false;
}




bool Fighter::isInMelee()           //判断enemy位于范围内，暂时不会写
{
	return false;
}

void Fighter::hurt(INT32 damage)
{
	int currentDamage = damage;
	if (!isZeroSheild())
	{
		if (currentDamage >= shield)
		{
			currentDamage -= shield;
			shield = 0;
		}
		else
		{
			shield -= currentDamage;
			currentDamage = 0;
		}
	}

	if (currentDamage > 0)
	{
		curHitPoints -= currentDamage;
		canBeHurt = false;
		this->runAction(CCBlink::create(2.0f,10));
	}
		currentDamage = 0;

	if (curHitPoints <= 0)
		die();
}

void Fighter::updateCondition()
{
	auto nowTime = GetCurrentTime()/1000.f;

	if (isZeroSheild())
	{
		if (nowTime - lastTimeInjured > 3.0f)
		{
			if (nowTime - lastTimeRecover >= 1.0f)
			{
				curShield += 10;
				lastTimeRecover = GetCurrentTime() / 1000.f;
			}
		}
	}
	if (!canBeHurt)
	{
		if (nowTime - lastTimeInjured > 2.0f)
		{
			canBeHurt = true;
			runAction(Show::create());
		}
	}
}

Vec2 Fighter::updateDestination()       //
{
	Vec2 current = this->getPosition();
	switch (direction)
	{
	case EDirection::UP:
		current.y += moveSpeed;
		break;
	case EDirection::UPLEFT:
		current.x -= moveSpeed;
		current.y += moveSpeed;
		break;
	case EDirection::UPRIGHT:
		current.x += moveSpeed;
		current.y += moveSpeed;
		break;
	case EDirection::LEFT:
		current.x -= moveSpeed;
		break;
	case EDirection::DOWN:
		current.y -= moveSpeed;
		break;
	case EDirection::DOWNLEFT:
		current.x -= moveSpeed;
		current.y -= moveSpeed;
		break;
	case EDirection::DOWNRIGHT:
		current.x += moveSpeed;
		current.y -= moveSpeed;
		break;
	case EDirection::RIGHT:
		current.x += moveSpeed;
		break;
	case EDirection::NODIR:
		break;
	default:
		break;
	}
	switch (fdirection)
	{
	case EDirection::UP:
		setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sUp.png", fighterName.getCString())->getCString());
		break;
	case EDirection::DOWN:
		setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sDown.png", fighterName.getCString())->getCString());
		break;
	case EDirection::LEFT:
		setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sLeft.png", fighterName.getCString())->getCString());
		break;
	case EDirection::RIGHT:
		setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sRight.png", fighterName.getCString())->getCString());
		break;
	}

	return current;
}

void Fighter::fighterMove(Vec2 newPosition)
{
	this->setPosition(newPosition);
}

void Fighter::stand()
{
	isMoving = false;
	switch (fdirection)
	{
	case EDirection::UP:
		setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sUp.png",fighterName.getCString())->getCString());
		break;
	case EDirection::DOWN:
		setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sDown.png", fighterName.getCString())->getCString());
		break;
	case EDirection::LEFT:
		setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sLeft.png", fighterName.getCString())->getCString());
		break;
	case EDirection::RIGHT:
		setTexture(CCString::createWithFormat("ArtDesigning/Sprite/Fighter/%sRight.png", fighterName.getCString())->getCString());
		break;
	}
	direction = EDirection::NODIR;
}

void Fighter::updateTarget()
{
	MovingActor* tempTarget = NULL;
	Vector<Enemy*>& allEnemySoldier = exploreScene->enemySoldier;
	Vector<Enemy*>& allEnemyBoss = exploreScene->enemyBoss;

	if (allEnemySoldier.empty() && allEnemyBoss.empty())
	{
		attackTarget = NULL;
		return;
	}
	if (!allEnemyBoss.empty())
	{
		auto temp = allEnemyBoss.begin();
		if (!(*temp)->getAlreadyDead() && !allEnemyBoss.empty())
		{
			tempTarget = *temp;
		}
	}
	if (!allEnemySoldier.empty())
	{
		if (!tempTarget)
		{
			float tempRadius = identityRadius;
			for (auto tempSoldier = allEnemySoldier.begin(); tempSoldier != allEnemySoldier.end(); ++tempSoldier)
			{
				float calRadius = (*tempSoldier)->getPosition().getDistance(this->getPosition());
				if (calRadius < tempRadius)
				{
					tempTarget = *tempSoldier;
					tempRadius = calRadius;
				}
			}
		}
	}
	attackTarget = tempTarget;
}


void Fighter::playAttackAnimation()
{
	//
}


bool Fighter::isZeroSheild()
{
	if (curShield == 0 && curHitPoints != 0)
	{
		return true;
	}
	return false;
}

void Fighter::getWeapon(Equipment* available)
{
	if (myWeapon.size() == equipNumber)
	{
		for (auto it = myWeapon.begin(); it != myWeapon.end(); it++)
			if (currentWeapon == *it)
			{
				(*it)->setNowState(WeaponStatus::GROUND);
				exploreScene->allWeapon.pushBack(*it);
				*it = available;
			}
	}
	else
	{
		myWeapon.pushBack(available);
		currentWeapon = available;
	}
}


void Fighter::die()
{

	//添加英雄死亡时的音效和图像
	//if(this == _combatScene->getMyHero())
	//{
	//	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Audio/YouHaveBeenSlained.wav", false, 1, 0, 1.2);
	//}
	//学长的参考代码
	
	setVisible(false);
	alreadyDead = true;
	//进入结算页面
}

void Fighter::releaseSkill()
{
     //继承下至具体英雄写	
}

void Fighter::bindSprite(CCSprite* sprite) {
	
	this->m_sprite = sprite;
	this->addChild(m_sprite);
}

void Fighter::takeBuff(Buff* buff)
{
	this->curHitPoints += buff->getBuffHp();
	this->curManaPoints += buff->getBuffMp();
	this->moveSpeed += buff->getBuffMoveSpeed();

	if (state == EBuffType::NORMAL)
		state = buff->getBuffType();

	buff->setBeginTime(GetCurrentTime());

	this->myBuff.pushBack(buff);
}

void Fighter::removeBuff()
{
	auto nowTime = GetCurrentTime();

	for (auto it = myBuff.begin(); it != myBuff.end();)
	{
		if (nowTime - (*it)->getBeginTime() > (*it)->getDuration())
		{
			
				this->curHitPoints -= (*it)->getBuffHp();
				this->curManaPoints -= (*it)->getBuffMp();
				this->moveSpeed -= (*it)->getBuffMoveSpeed();

				if (state == (*it)->getBuffType())
					state = NORMAL;
				it = myBuff.erase(it);
		}
		else
		{
			++it;
		}
	}
}