#include"MovingActor/Enemy.h"
#include"Enemy/Dragon.h"
#include"MovingActor/Constant.h"
#include"MovingActor/Bullet.h"
#include"MovingActor/Buff.h"
#include"Scene/GameScene.h"
#include"MovingActor/Constant.h"
#include<vector>

Dragon* Dragon::create(GameScene* Scene, std::string Name)
{
	Dragon* enemy = new Dragon();
	if (enemy && enemy->init(Scene, Name));
	{
		enemy->autorelease();
		return enemy;
	}

	CC_SAFE_DELETE(enemy);
	return nullptr;
}

bool Dragon::init(GameScene* Scene, std::string Name)
{
	if (!Enemy::init(Scene,Name))
		return false;

	initData(Scene, Name);
	return true;
}

bool Dragon::initData(GameScene* Scene, std::string Name)
{
	exploreScene = Scene;
	enemyName = Name;
	camp = AllCamp::ENEMY;
	level = EnemyLevel::BOSS;

	hitPoints = DRAGON_HP;
	attackSpeed = COMMON_ATTACKSPEED;
	curHitPoints = hitPoints;
	attackRadius = INIT_ID_RADIUS;
	damageAbility = DRAGON_DAMAGE;
	moveSpeed = DRAGON_MOVESPEED;
	identityRadius = INIT_ID_RADIUS;
	flySpeed = DRAGON_FLYSPEED;

	alreadyDead = false;
	everAttack = false;
	isReleaseSkill_1 = false;
	isReleaseSkill_2 = false;
	isReleaseSkill_3 = false;
	lastSkill = NOTHING;

	setTexture("ArtDesigning/Sprite/Enemy/Dragon/dragon1.png");
	loadAnimation();
	this->runAction(normal);

	return true;
}

bool Dragon::loadAnimation()
{
	Animation* normalAnimation = Animation::create();
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Enemy/Dragon/dragon%d.png", i);
		normalAnimation->addSpriteFrameWithFileName(frameName->getCString());

	}
	normalAnimation->setDelayPerUnit(1.0f / 12.0f);
	this->setNormal(CCRepeatForever::create(CCAnimate::create(normalAnimation)));
	
	return true;
}

void Dragon::chaosBullets()
{
			auto angle = random(0, 361);
			auto bulletSprite = Bullet::create("ArtDesigning/FlyingItem/Bullet/DragonBullet.png", damageAbility, flySpeed, this, NULL);
			//bulletSprite->setRotation(360 - angle);
			bulletSprite->setAngle(angle);

			auto fire = Buff::create(BURN,0,0,0,2.0f);
			bulletSprite->setcarryBuff(fire);

			exploreScene->getMap()->addChild(bulletSprite);
			exploreScene->getMap()->addChild(fire);
			exploreScene->flyingItem.pushBack(bulletSprite);
			exploreScene->allBuff.pushBack(fire);
}

void Dragon::roundBullets()
{
	auto miniAngle = random(0, 18);
	for (int i = 0; i < 20; i++)
	{
		auto angle = 18 * i + miniAngle;
		auto bulletSprite = Bullet::create("ArtDesigning/FlyingItem/Bullet/DragonBullet.png", damageAbility, flySpeed, this, NULL);	
		bulletSprite->setAngle(angle);
		auto fire = Buff::create(BURN, 0, 0, 0, 2.0f);
		bulletSprite->setcarryBuff(fire);
		exploreScene->getMap()->addChild(bulletSprite);
		exploreScene->getMap()->addChild(fire);
		exploreScene->allBuff.pushBack(fire);
		exploreScene->flyingItem.pushBack(bulletSprite);
	}
}

void Dragon::groundFlame()
{
	//技能动画生成
	Animation* bomb = Animation::create();
	for (int i = 1; i <= 2; i++)
	{
		auto Name = String::createWithFormat("ArtDesigning/FlyingItem/Bullet/bomb%d.png",i);
		bomb->addSpriteFrameWithFileName(Name->getCString());
	}
	bomb->setDelayPerUnit(0.5f);
	Animate* action = Animate::create(bomb);
	auto boom = Sequence::create(DelayTime::create(1.0f),
								 Repeat::create(action, 1), 
								 Hide::create(),
								 NULL);

	//伤害部分
	auto flame = Bullet::create("ArtDesigning/FlyingItem/Bullet/Flame.png", damageAbility, 5, this, NULL);
	flame->setPosition(exploreScene->getMyFighter()->getPosition());
	flame->runAction(boom);
	flame->setGiveOutTime(GetCurrentTime()/1000.f);
	exploreScene->specialBullet.pushBack(flame);
	exploreScene->getMap()->addChild(flame);
	//exploreScene->flyingItem.pushBack(flash);
}

void Dragon::updateState()
{
	auto nowTime = GetCurrentTime()/1000.f;

	if (!isReleaseSkill_1 && !isReleaseSkill_2 && !isReleaseSkill_3)
	{
		updateAction();
		if (lastSkill == NOTHING || (lastSkill == THREE && nowTime - endTime > 3.0f))
		{
			isReleaseSkill_1 = true;
			startTime = GetCurrentTime()/1000.f;
		}
		else if (lastSkill == ONE && nowTime - endTime > 3.0f)
		{
			isReleaseSkill_2 = true;
			startTime = GetCurrentTime()/1000.f;
		}
		else if (lastSkill == TWO && nowTime - endTime > 3.0f)
		{
			isReleaseSkill_3 = true;
			startTime = GetCurrentTime()/1000.f;
		}
		eachTime = GetCurrentTime()/1000.f;
	}

	if (isReleaseSkill_1)
	{
		if (fabs(nowTime - eachTime) > 0.005f)
		{
			chaosBullets();
			eachTime = GetCurrentTime()/1000.f;
		}

		if (nowTime - startTime >= 6.0f)
		{
			isReleaseSkill_1 = false;
			lastSkill = ONE;
			endTime = GetCurrentTime()/1000.f;
		}
	}

	if (isReleaseSkill_2)
	{
		if (fabs(nowTime - eachTime) > 0.5f)
		{
			roundBullets();
			eachTime = GetCurrentTime()/1000.f;
		}

		if (nowTime - startTime >= 6.0f)
		{
			isReleaseSkill_2 = false;
			lastSkill = TWO;
			endTime = GetCurrentTime()/1000.f;
		}
	}

	if (isReleaseSkill_3)
	{
		if (fabs(nowTime - eachTime) > 1.5f)
		{
			groundFlame();
			eachTime = GetCurrentTime()/1000.f;
		}

		if (nowTime - startTime >= 6.0f)
		{
			isReleaseSkill_3 = false;
			lastSkill = THREE;
			endTime = GetCurrentTime()/1000.f;
		}
	}
}