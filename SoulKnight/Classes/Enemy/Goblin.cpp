#include"MovingActor/Enemy.h"
#include"Enemy/Goblin.h"
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

	setTexture("ArtDesigning/Sprite/Enemy/Goblin/Goblin1.png");

	alreadyDead = false;
	attackSpeed = 0.f;

	curHitPoints = hitPoints;
	attackRadius = 500;
	level = SOLDIER;


	this->runAction(normal);

	return true;
}

bool Goblin::loadAnimation()
{
	cocos2d::Vector<SpriteFrame*> normalFrames;
	for (int i = 0; i < 2; i++)
	{
		SpriteFrame* frame = NULL;
		frame = SpriteFrameCache::sharedSpriteFrameCache()->
			spriteFrameByName(CCString::createWithFormat("ArtDesigning/Sprite/Enemy/Goblin/Goblin%d.png", i)->
				getCString());
		normalFrames.pushBack(normalFrames);
	}
	CCAnimation* normalAnimation = NULL;
	normalAnimation = CCAnimation::createWithSpriteFrames(normalFrames, 1.0 / 15.0);
	this->setNormal(CCRepeatForever::create(CCAnimate::create(normalAnimation)));

	return true;
}


bool Goblin::attack()
{

	if (attackTarget)
	{
		auto bulletSprite = Bullet::create("ArtDesigning/FlyingItem/Bullet/GoblinBullet.png", damageAbility, flySpeed, this, attackTarget);

		//对飞行物的调整
		bulletSprite->setPosition(this->getPosition());
		//bulletSprite->setScale();
		auto fire = Buff::create(EBuffType::POISON, 0, 0, 0, 4.0f);
		bulletSprite->setcarryBuff(fire);
		//将飞行物放入场景的容器之中
		exploreScene->getMap()->addChild(bulletSprite);
		exploreScene->flyingItem.pushBack(bulletSprite);

		return true;
	}
	return true;
}