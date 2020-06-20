#include"MovingActor/Enemy.h"
#include"MovingActor/Constant.h"
#include"MovingActor/Bullet.h"
#include"Scene/GameScene.h"
#include<random>

Enemy*Enemy::create(GameScene* Scene,std::string Name)
{
	Enemy* enemy = new Enemy();
	if (enemy && enemy->init(Scene, Name));
	{
		enemy->autorelease();
		return enemy;
	}

	CC_SAFE_DELETE(enemy);
	return nullptr;
}

bool Enemy::init(GameScene* Scene, std::string Name)
{
	if (!Sprite::init())
		return false;
	initData(Scene,Name);
	return true;
}

bool Enemy::initData(GameScene* Scene, std::string Name)
{
	exploreScene = Scene;
	enemyName = Name;
	camp = AllCamp::ENEMY;
	
	setTexture(CCString::createWithFormat("%s",enemyName)->getCString());

	isMoving = false;
	alreadyDead = false;
	attackSpeed = 0.5f;

	curHitPoints = hitPoints;
	attackRadius = 500;
	return true;
}

void  Enemy::updateTarget()
{
	MovingActor* tempTarget = NULL;
	Vector<MovingActor*>& allFighter = exploreScene->allFighter;

	//先按照只有一名玩家处理
	auto temp = allFighter.begin();
	auto tempRadius = identityRadius;


	if (!(*temp)->getAlreadyDead() && tempRadius > (*temp)->getPosition().getDistance(this->getPosition()))
		tempTarget = *temp;


	attackTarget = tempTarget;
}

void Enemy::updateDestination()
{
	Vec2 tempDes;

		float tempX, tempY;

		tempX = random(-30, 30) ;//如果过大，将出界，因为界外没有瓦片,不对其进行碰撞检测，直接框在房间里
		tempY = random(-30, 30) ;

		tempDes.x = getPosition().x + tempX;
		tempDes.y = getPosition().y + tempY;
	
	if(attackTarget&&level!=BOSS)
	{
		if (attackTarget->getPosition() == getPosition())
		{
			destination = tempDes;
			return;
		}
		if (attackTarget->getPosition().getDistance(this->getPosition()) > attackRadius)
		{
			tempDes = attackTarget->getPosition();
		}
	}
	destination = tempDes;
}

void Enemy::enemyMove()
{
	isMoving = true;
	auto moveTime = destination.getDistance(getPosition()) / moveSpeed;
	auto move = MoveTo::create(moveTime,this->getDestination());
	move->setTag(982);
	this->runAction(move);
}

bool Enemy::attack()
{

	if (attackTarget)
	{
		auto bulletSprite = Bullet::create(CCString::createWithFormat("%sBullet",enemyName)->getCString(), damageAbility, flySpeed, this, attackTarget);
		
		//对飞行物的调整
		bulletSprite->setPosition(this->getPosition());
		//bulletSprite->setScale();
		
		//将飞行物放入场景的容器之中
		exploreScene->getMap()->addChild(bulletSprite);
		exploreScene->flyingItem.pushBack(bulletSprite);
		
		return true;
	}
	return false;
}

void Enemy::die()
{
	setVisible(false);
	exploreScene->setCoinNum(exploreScene->getCoinNum()+5);
	alreadyDead = true;
}
void Enemy::updateAction()
{
	auto nowTime = GetCurrentTime()/1000.f;
	if (everAttack)
		if (nowTime - lastAttackTime < attackSpeed)
			return;

	if (!isToMove)
	{
		isMoving = false;
		stopActionByTag(982);
	}
	updateTarget();
	if (attackTarget)
	{
		if (!attackTarget->getAlreadyDead())
		{
			auto distance = attackTarget->getPosition().getDistance(this->getPosition());

			if (distance < attackRadius)
			{
				attack();
				everAttack = true;
				lastAttackTime = GetCurrentTime()/1000.f;
			}
			if (distance < attackRadius - 50&&attackMode==REMOTE)
				isToMove = false;
			else
				isToMove = true;
		}
	}
}
void Enemy::bindSprite(CCSprite* sprite) {
	this->m_sprite = sprite;
	this->addChild(m_sprite);
}
