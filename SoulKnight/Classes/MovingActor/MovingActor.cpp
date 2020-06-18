#include"MovingActor/MovingActor.h"
#include"MovingActor/Constant.h"
#include"MovingActor/Bullet.h"
#include"Scene/GameScene.h"

//�������������

MovingActor* MovingActor::create(const std::string& filename, GameScene* Scene)
{
	MovingActor* sprite = new  MovingActor();
	if (sprite && sprite->init(filename, Scene))
	{
		sprite->autorelease();
		return sprite;
	}

	CC_SAFE_DELETE(sprite);
	return nullptr;
}


bool MovingActor::init(const std::string& filename, GameScene* Scene)
{
	if (!Sprite::initWithFile(filename))
	{
		return false;
	}

	initData(Scene);

	return true;
}

void MovingActor::initData(GameScene* Scene)
{
	exploreScene = Scene;
	hitPoints = INIT_HITPOINTS;      //��ʼ����ֵ5
	curHitPoints = hitPoints;       //��ǰ����ֵΪ��ֵ
	identityRadius = INIT_ID_RADIUS;//��ʼ��֪�뾶500��boss���ܻ����
	moveSpeed = INIT_MOVESPEED; 
	attackSpeed = 0.f;           //���Ǻ�enemy�������Ʋ�ͬ����ʱ��Ϊ0
	alreadyDead = false;


}

void MovingActor::takeDamage(DamageMode type, INT32 damage, MovingActor* enemy)
{

	curHitPoints -= damage;
	attackFrom = enemy;
	CCLOG("BOSS HP=%d", curHitPoints);
	if (curHitPoints<= 0)
		die();
}

bool MovingActor::attack()
{
	//��׼������Щ�����ᵽFighter��enemy����
	return false;
}

void MovingActor::die()
{
	alreadyDead = true;
}

//buff������