#include"MovingActor/Enemy.h"
#include"Enemy/Pig.h"
#include"MovingActor/Buff.h"
#include"MovingActor/Constant.h"
#include"MovingActor/Bullet.h"
#include"Scene/GameScene.h"
#include<vector>

Pig* Pig::create(GameScene* Scene, std::string Name)
{
	Pig* enemy = new Pig();
	if (enemy && enemy->init(Scene, Name));
	{
		enemy->autorelease();
		return enemy;
	}

	CC_SAFE_DELETE(enemy);
	return nullptr;
}

bool Pig::init(GameScene* Scene, std::string Name)
{
	if (!Enemy::init(Scene, Name))
		return false;

	initData(Scene, Name);
	return true;
}

bool Pig::initData(GameScene* Scene, std::string Name)
{
	exploreScene = Scene;
	enemyName = Name;
	camp = AllCamp::ENEMY;
	level = SOLDIER;
	attackMode = MELEE;

	attackSpeed = PIG_ATTACKSPEED;
	hitPoints = PIG_HP;
	curHitPoints = hitPoints;    //数据具体化，不动用plist

	damageAbility = PIG_DAMAGE;
	moveSpeed = PIG_MOVESPEED;
	identityRadius = PIG_IDR;
	attackRadius = PIG_ATTACKR;

	alreadyDead = false;
	everAttack = false;


	setTexture("ArtDesigning/Sprite/Enemy/Pig/Pig1.png");
	loadAnimation();
	this->runAction(normal);

	return true;
}

bool Pig::loadAnimation()
{
	Animation* normalAnimation = Animation::create();
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Enemy/Pig/Pig%d.png", i);
		normalAnimation->addSpriteFrameWithFileName(frameName->getCString());

	}
	normalAnimation->setDelayPerUnit(1.0f / 12.0f);
	this->setNormal(CCRepeatForever::create(CCAnimate::create(normalAnimation)));

	return true;
}


bool Pig::attack()
{

	if (attackTarget)
	{
		float angle;
		if (attackTarget)
		{
			auto victimVector = attackTarget->getPosition() - getPosition();
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
		auto damage = Bullet::create("ArtDesigning/FlyingItem/Bullet/PigEffect.png",
			damageAbility,
			0,
			this,
			NULL);
		damage->setAttackMode(MELEE);
		damage->setScaleX(0.25);
		damage->setPosition(this->getPosition().x + 20 * cos(angle / 180 * M_PI),
			this->getPosition().y + 20 * sin(angle / 180 * M_PI));
		damage->setGiveOutTime(GetCurrentTime() / 1000.f);
		if (angle >= 90 && angle <= 270)
			damage->setScaleX(-1.0);
		exploreScene->specialBullet.pushBack(damage);
		exploreScene->getMap()->addChild(damage);
		return true;
	}
	return false;
}