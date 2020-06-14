#include"EnemyRemote.h"
#include "Scene/GameScene.h"
#include"Bullet.h"
#include"Fighter.h"


EnemyRemote* EnemyRemote::create(GameScene* Scene, std::string enemyName)
{


	EnemyRemote* enemyRemote = new EnemyRemote();
	if (enemyRemote && enemyRemote->init(Scene, enemyName))
	{
		enemyRemote->autorelease();
		return enemyRemote;
	}
	CC_SAFE_DELETE(enemyRemote);
	return nullptr;
}



bool EnemyRemote::init(GameScene* Scene, std::string enemyName)
{
	if (!Enemy::init(Scene, enemyName))
	{
		return false;
	}

	initData(Scene, enemyName);
	return true;
}



bool EnemyRemote::initData(GameScene* Scene, std::string enemyName)
{
	enemyName = enemyName;

	attackSpeed = 0.5f;
	hitPoints = 20;
	curHitPoints = hitPoints;    //数据具体化，不动用plist

	damageAbility = 2;
	moveSpeed = 1.0;
	identityRadius = 500;

	alreadyDead = false;

	lastAttackTime = 0.f;
	return false;
}





bool EnemyRemote::attack()
{
	auto nowTime = GetCurrentTime() / 1000.f;

	//如果在远程攻击对象范围以内，则触发攻击动画，切必须要在上一次攻击的时间间隔之后
	//待写

	if (attackTarget&& nowTime - lastAttackTime > (1.0f / attackSpeed))
	{
		auto bulletSprite = Bullet::create(CCString::createWithFormat("%sBullet", enemyName)->getCString(), damageAbility, flySpeed, this, attackTarget);

		//对飞行物的调整
		bulletSprite->setPosition(this->getPosition());
		//bulletSprite->setScale();

		//将飞行物放入场景的容器之中
		exploreScene->getMap()->addChild(bulletSprite);
		exploreScene->flyingItem.pushBack(bulletSprite);

		return true;
	}
	return true;
}


