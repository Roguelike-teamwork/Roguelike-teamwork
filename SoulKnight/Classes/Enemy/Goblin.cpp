#include"MovingActor/Enemy.h"
#include"Enemy/Goblin.h"
#include"MovingActor/Buff.h"
#include"MovingActor/Constant.h"
#include"MovingActor/Bullet.h"
#include"Scene/GameScene.h"
#include<vector>

Goblin* Goblin::create(GameScene* Scene, std::string Name)
{
	Goblin* enemy = new Goblin();
	if (enemy && enemy->init(Scene, Name));
	{
		enemy->autorelease();
		return enemy;
	}

	CC_SAFE_DELETE(enemy);
	return nullptr;
}

bool Goblin::init(GameScene* Scene, std::string Name)
{
	if (!Enemy::init(Scene, Name))
		return false;

	initData(Scene, Name);
	return true;
}

bool Goblin::initData(GameScene* Scene, std::string Name)
{
	exploreScene = Scene;
	enemyName = Name;
	camp = AllCamp::ENEMY;
	level = SOLDIER;



	attackSpeed = GOBLIN_ATTACKSPEED;
	hitPoints = GOBLIN_HP;
	curHitPoints = hitPoints;    //数据具体化，不动用plist
	damageAbility = GOBLIN_DAMAGE;
	moveSpeed = GOBLIN_MOVESPEED;
	identityRadius = GOBLIN_IDR;
	attackRadius = GOBLIN_ATTACKR;

	alreadyDead = false;
	everAttack = false;

	setTexture("ArtDesigning/Sprite/Enemy/Goblin/Goblin1.png");
	loadAnimation();
	this->runAction(normal);

	return true;
}

bool Goblin::loadAnimation()
{
	Animation* normalAnimation = Animation::create();
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Enemy/Goblin/Goblin%d.png",i);
		normalAnimation->addSpriteFrameWithFileName(frameName->getCString());

	}
	normalAnimation->setDelayPerUnit(1.0f / 12.0f);
	this->setNormal(CCRepeatForever::create(CCAnimate::create(normalAnimation)));

	return true;
}


bool Goblin::attack()
{

	if (attackTarget)
	{
		auto bulletSprite = Bullet::create("ArtDesigning/FlyingItem/Bullet/GoblinBullet.png", damageAbility, 5, this, attackTarget);

		//对飞行物的调整
		bulletSprite->setPosition(this->getPosition());
		//bulletSprite->setScale();
		auto fire = Buff::create(EBuffType::POISON, 0, 0, 0, 4.0f);
		bulletSprite->giveOut();
		bulletSprite->setcarryBuff(fire);
		//将飞行物放入场景的容器之中
		exploreScene->getMap()->addChild(bulletSprite);
		exploreScene->flyingItem.pushBack(bulletSprite);

		return true;
	}
	return true;
}