#include"EnemyMelee.h"
#include "Scene/GameScene.h"
#include"Fighter.h"
EnemyMelee* EnemyMelee::create(GameScene* Scene, std::string enemyName)
{


	EnemyMelee* enemyMelee = new EnemyMelee();
	if (enemyMelee && enemyMelee->init( Scene,  enemyName))
	{
		enemyMelee->autorelease();
		return enemyMelee;
	}
	CC_SAFE_DELETE(enemyMelee);
	return nullptr;
}

bool EnemyMelee::init(GameScene* Scene, std::string enemyName)
{
	if (!Enemy::init(Scene, enemyName))
	{
		return false;
	}

	initData(Scene, enemyName);
	return true;
}

bool EnemyMelee::initData(GameScene* Scene, std::string enemyName)
{
	enemyName = enemyName;

	attackSpeed = 1.0f;
	hitPoints = 50;
	curHitPoints = hitPoints;    //数据具体化，不动用plist

	damageAbility = 1;
	moveSpeed = 100;
	identityRadius = 400;

	alreadyDead = false;

	lastAttackTime = 0.f;
	return false;
}



bool  EnemyMelee::attack()
{
	auto nowTime = GetCurrentTime() / 1000.f;

	if (attackTarget)
	{
		//如果近身之攻击对象范围以内，则触发攻击动画，切必须要在上一次攻击的时间间隔之后
		if (nowTime - lastAttackTime > (1.0f / attackSpeed))
		{
			if ((Fighter*)attackTarget/*->curHitPoints*/)//如果Fighter吃伤害，根据是否有护盾判断掉血.此处attackTarget不是fighter类的，我想强转但可能有问题
			{
			}

		}

	}
	return true;
	
}

