#include "Comp.h"
#include "MovingActor/MovingActor.h"
#include "Scene/GameScene.h"

Comp* Comp::create(MovingActor* sprite)
{
	Comp* comp = new Comp();
	if (comp && comp->init(sprite))
	{
		comp->autorelease();
		return comp;
	}
	CC_SAFE_RELEASE(comp);
	return NULL;
}

bool Comp::init(MovingActor* sprite)
{
	owner = sprite;
	auto red = Sprite::create("ArtDesigning/Word&Others/others/RedComp.png");
	blood = ProgressTimer::create(red);
	blood->setType(ProgressTimer::Type::BAR);
	blood->setPosition(Vec2(owner->getPosition().x,owner->getPosition().y+50));
	blood->setMidpoint(Vec2(0, 0.5));
	blood->setBarChangeRate(Vec2(1, 0));
	owner->getExploreScene()->getMap()->addChild(blood);
	return true;
}

void Comp::updateState()
{
	if (owner->getAlreadyDead())
	{
		owner = NULL;
		return;
	}
	blood->setPercentage((((float)owner->getCurHitPoints()) / owner->getHitpoints()) * 100);
	blood->setPosition(Vec2(owner->getPosition().x, owner->getPosition().y + 50));
}