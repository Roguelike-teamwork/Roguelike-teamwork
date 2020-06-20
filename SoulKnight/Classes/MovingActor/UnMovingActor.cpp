#include"UnMovingActor.h"
#include"Scene/GameScene.h"
#include"Weapon/Fork.h"




UnMovingActor* UnMovingActor::create(const std::string& filename, GameScene* Scene, TOTALNPC NPC, float radius)
{
	UnMovingActor* unmovingactor = new UnMovingActor();
	if (unmovingactor && unmovingactor->init(filename, Scene,NPC,radius))
	{
		unmovingactor->autorelease();
		return unmovingactor;
	}

	CC_SAFE_DELETE(unmovingactor);
	return nullptr;

}

bool UnMovingActor::init(const std::string& filename, GameScene* Scene, TOTALNPC NPC, float radius)
{
	if (!Sprite::init())
	{
		return false;
	}

	npcName = filename;
	npcType = NPC;
	isConfirm = false;
	isOver = false;
	isMeet = false;
	_spawnScene = Scene;
	touchRadius = radius;

	setTexture(String::createWithFormat("ArtDesigning/Sprite/NPC/%s_1.png",npcName.getCString())->getCString());

	
	putMessages = Label::create("","Marker Felt",30);
	putMessages->setPosition(400,70);
	//putMessages->runAction(Hide::create());
	_spawnScene->addChild(putMessages,5);
	
	return true;
}

bool UnMovingActor::isInRadius(Fighter* fighter)  //Ð´¾àÀë
{
	if (fighter->getPosition().getDistance(this->getPosition()) < touchRadius)
		return true;
	return false;
}


void UnMovingActor::effect(Fighter* customer)
{
	auto action = Sequence::create(
		Show::create(),
		DelayTime::create(3.0f),
		Hide::create(),
		NULL
	);
	putMessages->runAction(action);

	if (npcType == Businessman)
	{
		if (!isMeet)
		{
			goodsType = random(0,2);
			isMeet = true;
		}

		if (!isOver)
		{
			if (goodsType == 0)
			{
				if (!isConfirm)
					putMessages->setString("HP 15Gold");
				else
				{
					_spawnScene->setCoinNum(_spawnScene->getCoinNum() - 15);
					customer->takeBuff(Buff::create(EBuffType::NORMAL,150,0,0,5000));
					isOver = true;
				}
			}
			else if (goodsType == 1)
			{
				if (!isConfirm)
					putMessages->setString("Energy 15Gold");
				else
				{
					_spawnScene->setCoinNum(_spawnScene->getCoinNum() - 15);
					customer->takeBuff(Buff::create(EBuffType::NORMAL, 0, 300, 0, 5000));
					isOver = true;
				}
			}
			else if (goodsType == 2)
			{
				if(!isConfirm)
					putMessages->setString("Fork 50Gold");
				else
				{
					_spawnScene->setCoinNum(_spawnScene->getCoinNum() - 50);
					auto goods = Fork::create(MELEE,"Fork",4,0.5f,100,2);
					goods->setPosition(getPosition().x-50,getPosition().y-50);
					_spawnScene->getMap()->addChild(goods);
					_spawnScene->allWeapon.pushBack(goods);
					isOver = true;
				}
			}
		}
		else
		{
			putMessages->setString("See You!!");
		}
	}
	else if (npcType == Statue)
	{
		if (!isMeet)
		{
			goodsType = random(0, 1);
			isMeet = true;
		}
		if (!isOver)
		{
			if (goodsType == 0)
			{
				if (!isConfirm)
					putMessages->setString("SpeedUp 25Gold");
				else
				{
					_spawnScene->setCoinNum(_spawnScene->getCoinNum() - 50);
					customer->takeBuff(Buff::create(EBuffType::SPEEDUP,0,0,2,5000));
					isOver = true;
				}
			}
			if (goodsType == 1)
			{

				if (!isConfirm)
					putMessages->setString("Fire if will 50Gold ");
				else
				{
					_spawnScene->setCoinNum(_spawnScene->getCoinNum() -50);
					customer->getCurrentWeapon()->setAttackSpeedNumber(customer->getCurrentWeapon()->getAttackSpeedNumber() / 2);
					isOver = true;
				}

			}
		}
		else
		{
			putMessages->setString("I'm Exhausted,just leave me along");
		}
	}
	else if (npcType == CoinBox)
	{
		if (!isOver)
		{
			if (!isConfirm)
				putMessages->setString("You see something Gold inside");
			else
			{
				_spawnScene->setCoinNum(_spawnScene->getCoinNum()+100);
				setTexture(String::createWithFormat("ArtDesigning/Sprite/NPC/%s_2.png", npcName.getCString())->getCString());
				isOver = true;
			}
		}
		else
		{
			putMessages->setString("You take them,it full you with Determination");
		}
	}
	else if (npcType == BloodBox)
	{
		if (!isOver)
		{
			if (!isConfirm)
				putMessages->setString("Oh my god,it is bleeding");
			else
			{
				customer->takeBuff(Buff::create(NORMAL,customer->getHitpoints()-customer->getCurHitPoints(),0,0,5000));
				setTexture(String::createWithFormat("ArtDesigning/Sprite/NPC/%s_2.png", npcName.getCString())->getCString());
				isOver = true;
			}
		}
		else
		{
			putMessages->setString("You Drink it,something recover");
		}
	}
	else if (npcType == ManaBox)
	{
	if (!isOver)
	{
		if (!isConfirm)
			putMessages->setString("Something shinning inside");
		else
		{
			customer->takeBuff(Buff::create(NORMAL,0,customer->getManaPoints()-customer->getCurManaPoints(), 0, 5000));
			setTexture(String::createWithFormat("ArtDesigning/Sprite/NPC/%s_2.png", npcName.getCString())->getCString());
			isOver = true;
		}
	}
	else
	{
		putMessages->setString("You take them,it make you shinning");
	}
	}
}



Equipment* UnMovingActor::equipBack()
{
	return nullptr;
}





