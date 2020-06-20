#include"MovingActor/Enemy.h"
#include"Enemy/Crow.h"
#include"MovingActor/Buff.h"
#include"MovingActor/Constant.h"
#include"MovingActor/Bullet.h"
#include"Scene/GameScene.h"
#include<vector>

Crow* Crow::create(GameScene* Scene, std::string Name)
{
	Crow* enemy = new Crow();
	if (enemy && enemy->init(Scene, Name));
	{
		enemy->autorelease();
		return enemy;
	}

	CC_SAFE_DELETE(enemy);
	return nullptr;
}

bool Crow::init(GameScene* Scene, std::string Name)
{
	if (!Enemy::init(Scene, Name))
		return false;

	initData(Scene, Name);
	return true;
}

bool Crow::initData(GameScene* Scene, std::string Name)
{
	exploreScene = Scene;
	enemyName = Name;
	camp = AllCamp::ENEMY;
	level = SOLDIER;
	attackMode = REMOTE;



	attackSpeed = CROW_ATTACKSPEED;
	hitPoints = CROW_HP;
	curHitPoints = hitPoints;    //数据具体化，不动用plist
	damageAbility = CROW_DAMAGE;
	moveSpeed = CROW_MOVESPEED;
	identityRadius = CROW_IDR;
	attackRadius = CROW_ATTACKR;

	alreadyDead = false;
	everAttack = false;

	setTexture("ArtDesigning/Sprite/Enemy/Crow/Crow1.png");
	loadAnimation();
	this->runAction(normal);

	return true;
}

bool Crow::loadAnimation()
{
	Animation* normalAnimation = Animation::create();
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Enemy/Crow/Crow%d.png", i);
		normalAnimation->addSpriteFrameWithFileName(frameName->getCString());

	}
	normalAnimation->setDelayPerUnit(1.0f / 12.0f);
	this->setNormal(CCRepeatForever::create(CCAnimate::create(normalAnimation)));

	return true;
}


bool Crow::attack()
{

	if (attackTarget)
	{
		auto bulletSprite = Bullet::create("ArtDesigning/FlyingItem/Bullet/CrowBullet.png", damageAbility, 5, this, attackTarget);

		//对飞行物的调整
		bulletSprite->setPosition(this->getPosition());
		//bulletSprite->setScale();
		auto fire = Buff::create(EBuffType::POISON, 0, 0, 0, 4.0f);
		bulletSprite->giveOut();
		bulletSprite->setcarryBuff(fire);
		bulletSprite->setScale(0.625);
		//将飞行物放入场景的容器之中
		exploreScene->getMap()->addChild(fire);
		exploreScene->getMap()->addChild(bulletSprite);
		exploreScene->flyingItem.pushBack(bulletSprite);
		exploreScene->allBuff.pushBack(fire);

		return true;
	}
	return true;
}