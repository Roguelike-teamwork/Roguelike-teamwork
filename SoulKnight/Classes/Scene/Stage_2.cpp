#include "Scene/Stage_2.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Controller/MoveCtrl.h"
#include "cocos2d.h"
#include "Comp/Comp.h"
#include "Enemy/Dragon.h"
#include"Enemy/Goblin.h"
#include"Enemy/Pig.h"
#include"MovingActor/Knight.h"
#include"MovingActor/EnemyMelee.h"
#include"Weapon/Pistol.h"
#include"Weapon/Fork.h"
#include"Scene/MenuScene.h"

Scene* Stage_2::createScene()
{
	return Stage_2::create();
}

Stage_2* Stage_2::create()
{
	auto scene = new Stage_2;
	if (scene && scene->init())
	{
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return NULL;
}


bool Stage_2::init()
{
	if (!Scene::init())
	{
		return false;
	}
	_gameBegin = true;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	roomState = SPAWNROOM_2;
	initMapLayer();

	initFighter();
	loadingAnimation();
	initComp();
	initListener();
	initNextScene();
	initSettingMenu();
	initCoin();
	initUI();
	initController();

	generateEnemies();
	generateNpc();
	generateComp();

	scheduleUpdate();
	return true;
}

void Stage_2::initMapLayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	_map = CCTMXTiledMap::create("ArtDesigning/SceneAndMap/GameMap/GameMap2/GameMap2.tmx");
	_map->setAnchorPoint(Vec2(0, 0));
	_map->setPosition(Vec2(-1700, -300));
	_collidable = _map->getLayer("collision");
	_collidable->setVisible(false);
	_fenceBool = _map->getLayer("fenceBool");
	_fenceBool->setVisible(false);
	_collisionForFight = _map->getLayer("collisionForFight");
	addChild(_map, 0, 10000);//TAG_MAP
}

void Stage_2::generateEnemies()
{
	int temp;
	//第一房间
	for (int i = 0; i < 5; i++)
	{
		temp = random(0, 1);
		if (temp == 0)
		{
			Enemy* enemy = Goblin::create(this, "Goblin");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint");
			float tempX = playerPointDic.at("x").asFloat()+i*20;
			float tempY = playerPointDic.at("y").asFloat();
			enemy->setPosition(ccp(tempX, tempY));
			enemy->setEnemyState(ROOM_2_1);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);
		}
		else if (temp == 1)
		{
			Enemy* enemy = Pig::create(this, "Pig");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint");
			float tempX = playerPointDic.at("x").asFloat()+i*20;
			float tempY = playerPointDic.at("y").asFloat();
			enemy->setPosition(ccp(tempX, tempY));
			enemy->setEnemyState(ROOM_2_1);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		temp = random(0, 1);
		if (temp == 0)
		{
			Enemy* enemy = Goblin::create(this, "Goblin");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint1");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat() + i * 20;
			enemy->setPosition(ccp(tempX, tempY));
			enemy->setEnemyState(ROOM_2_1);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);
		}
		else if (temp == 1)
		{
			Enemy* enemy = Pig::create(this, "Pig");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint1");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat() + i * 20;
			enemy->setPosition(ccp(tempX, tempY));
			enemy->setEnemyState(ROOM_2_1);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		temp = random(0, 1);
		if (temp == 0)
		{
			Enemy* enemy = Goblin::create(this, "Goblin");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint3");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat() + i * 20;
			enemy->setPosition(ccp(tempX, tempY));
			enemy->setEnemyState(ROOM_2_3);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);
		}
		else if (temp == 1)
		{
			Enemy* enemy = Pig::create(this, "Pig");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint3");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat() + i * 20;
			enemy->setPosition(ccp(tempX, tempY));
			enemy->setEnemyState(ROOM_2_3);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		temp = random(0, 1);
		if (temp == 0)
		{
			Enemy* enemy = Goblin::create(this, "Goblin");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint4");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat() + i * 20;
			enemy->setPosition(ccp(tempX, tempY));
			enemy->setEnemyState(ROOM_2_3);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);
		}
		else if (temp == 1)
		{
			Enemy* enemy = Pig::create(this, "Pig");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint4");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat() + i * 20;
			enemy->setPosition(ccp(tempX, tempY));
			enemy->setEnemyState(ROOM_2_3);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);
		}
	}



	//生产boss
	Enemy* enemy = Dragon::create(this, "Dragon");

	//加载对象层
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	//加载玩家坐标对象
	ValueMap  playerPointDic = objGroup->objectNamed("EnemyPoint5");
	float tempX = playerPointDic.at("x").asFloat();
	float tempY = playerPointDic.at("y").asFloat();
	enemy->setPosition(ccp(tempX, tempY));
	enemy->setEnemyState(ROOM_2_3);
	_map->addChild(enemy, 1);
	enemyBoss.pushBack(enemy);
	allEnemy.pushBack(enemy);
}

void Stage_2::generateNpc()
{
	int temp = random(0, 4);

		if (temp == 0)
		{
			auto npc = UnMovingActor::create("Shop", this, Businessman, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint1");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();

			npc->setPosition(ccp(npcX, npcY));
			npc->setScale(0.75);
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		else if (temp == 1)
		{
			auto npc = UnMovingActor::create("Statue", this, Statue, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint1");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();

			npc->setPosition(ccp(npcX, npcY));
			npc->setScale(0.75);
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		else if (temp == 2)
		{
			auto npc = UnMovingActor::create("HPBOX", this, BloodBox, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint1");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();
			npc->setPosition(ccp(npcX, npcY));
			npc->setScale(1.25);
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		else if (temp == 3)
		{
			auto npc = UnMovingActor::create("EnergyBox", this, ManaBox, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint1");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();
			npc->setScale(0.75);
			npc->setPosition(ccp(npcX, npcY));
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		else if (temp == 4)
		{
			auto npc = UnMovingActor::create("CoinBox", this, CoinBox, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint1");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();
			npc->setScale(0.6);
			npc->setPosition(ccp(npcX, npcY));
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		temp = random(0, 4);
		if (temp == 0)
		{
			auto npc = UnMovingActor::create("Shop", this, Businessman, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint2");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();

			npc->setPosition(ccp(npcX, npcY));
			npc->setScale(0.75);
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		else if (temp == 1)
		{
			auto npc = UnMovingActor::create("Statue", this, Statue, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint2");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();

			npc->setPosition(ccp(npcX, npcY));
			npc->setScale(0.75);
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		else if (temp == 2)
		{
			auto npc = UnMovingActor::create("HPBOX", this, BloodBox, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint2");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();
			npc->setPosition(ccp(npcX, npcY));
			npc->setScale(1.25);
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		else if (temp == 3)
		{
			auto npc = UnMovingActor::create("EnergyBox", this, ManaBox, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint2");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();
			npc->setScale(0.75);
			npc->setPosition(ccp(npcX, npcY));
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
		else if (temp == 4)
		{
			auto npc = UnMovingActor::create("CoinBox", this, CoinBox, 75);
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("NPCPoint2");
			float npcX = playerPointDic.at("x").asFloat();
			float npcY = playerPointDic.at("y").asFloat();
			npc->setScale(0.6);
			npc->setPosition(ccp(npcX, npcY));
			getMap()->addChild(npc);
			allNpc.pushBack(npc);
		}
}

