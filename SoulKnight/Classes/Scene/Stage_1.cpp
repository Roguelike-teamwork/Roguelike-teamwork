#include "Scene/Stage_1.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Controller/MoveCtrl.h"
#include "cocos2d.h"
#include "Comp/Comp.h"
#include "Enemy/Dragon.h"
#include"Enemy/Goblin.h"
#include"Enemy/Pig.h"
#include"MovingActor/Knight.h"
#include"Weapon/Pistol.h"
#include"Weapon/Fork.h"
#include"Enemy/Rabbit.h"
#include"Enemy/Crow.h"
#include"Scene/MenuScene.h"

Scene* Stage_1::createScene(testmanSelect* scene)
{
	return Stage_1::create(scene);
}

Stage_1* Stage_1::create(testmanSelect* scene)
{
	auto _scene = new Stage_1;
	if (_scene && _scene->init(scene))
	{
		_scene->autorelease();
		return _scene;
	}
	CC_SAFE_DELETE(_scene);
	return NULL;
}


bool Stage_1::init(testmanSelect* scene)
{
	if (!Scene::init())
	{
		return false;
	}
	_gameBegin = true;
	setPtestmanSelect(scene);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	roomState = SPAWNROOM_1;
	initMapLayer();
	initFighter();
	loadingAnimation();
	initComp();
	initListener();
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

void Stage_1::initMapLayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	_map = CCTMXTiledMap::create("ArtDesigning/SceneAndMap/GameMap/GameMap1/GameMap1.tmx");
	_map->setAnchorPoint(Vec2(0, 0));
	_map->setPosition(Vec2(-200, -300));
	_collidable = _map->getLayer("collision");
	_collidable->setVisible(false);
	_fenceBool = _map->getLayer("fenceBool");
	_fenceBool->setVisible(false);
	_collisionForFight = _map->getLayer("collisionForFight");
	_collisionForFight->setVisible(false);
	_fence = _map->getLayer("fence");
	_fence->setVisible(false);
	_transfer = _map->getLayer("transfer");
	_transfer->setVisible(false);
	_spikeweed = _map->getLayer("spikeweed");
	addChild(_map, 0, 10000);//TAG_MAP
}

void Stage_1::generateEnemies()
{
	int temp;
	//第一房间
	for (int i = 0; i < 5; i++)
	{
		temp = random(0,3);
		if (temp == 0)
		{
			Enemy* enemy = Goblin::create(this, "Goblin");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("enemy1");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat();
			enemy->setPosition(ccp(tempX, tempY - i * 20));
			enemy->setEnemyState(ROOM_1_1);
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
				ValueMap  playerPointDic = objGroup->objectNamed("enemy1");
				float tempX = playerPointDic.at("x").asFloat();
				float tempY = playerPointDic.at("y").asFloat();
				enemy->setPosition(ccp(tempX, tempY - i * 20));
				enemy->setEnemyState(ROOM_1_1);
				_map->addChild(enemy, 1);
				enemySoldier.pushBack(enemy);
				allEnemy.pushBack(enemy);
			
		}
		else if (temp == 2)
		{
			Enemy* enemy = Rabbit::create(this, "Rabbit");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("enemy1");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat();
			enemy->setPosition(ccp(tempX, tempY - i * 20));
			enemy->setEnemyState(ROOM_1_1);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);

		}
		else if (temp == 3)
		{
			Enemy* enemy = Crow::create(this, "Crow");

			//加载对象层
			CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
			//加载玩家坐标对象
			ValueMap  playerPointDic = objGroup->objectNamed("enemy1");
			float tempX = playerPointDic.at("x").asFloat();
			float tempY = playerPointDic.at("y").asFloat();
			enemy->setPosition(ccp(tempX, tempY - i * 20));
			enemy->setEnemyState(ROOM_1_1);
			_map->addChild(enemy, 1);
			enemySoldier.pushBack(enemy);
			allEnemy.pushBack(enemy);

		}
	}
}

void Stage_1::generateNpc()
{
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			int temp = random(0,1);
			if (temp == 0)
			{
				auto npc = UnMovingActor::create("Shop", this, Businessman, 75);
				CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
				//加载玩家坐标对象
				ValueMap  playerPointDic = objGroup->objectNamed("npc1");
				float npcX = playerPointDic.at("x").asFloat();
				float npcY = playerPointDic.at("y").asFloat();

				npc->setPosition(ccp(npcX - (1 - 2 * i) * 100, npcY - (1 - 2 * i) * 100));
				npc->setScale(0.75);
				getMap()->addChild(npc);
				allNpc.pushBack(npc);
			}
			else if (temp == 1)
			{
				auto npc = UnMovingActor::create("Statue",this,Statue, 75);
				CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
				//加载玩家坐标对象
				ValueMap  playerPointDic = objGroup->objectNamed("npc1");
				float npcX = playerPointDic.at("x").asFloat();
				float npcY = playerPointDic.at("y").asFloat();

				npc->setPosition(ccp(npcX - (1 - 2 * i) * 100, npcY - (1 - 2 * i) * 100));
				npc->setScale(0.75);
				getMap()->addChild(npc);
				allNpc.pushBack(npc);
			}
		}
		else if (i == 1)
		{
			int temp = random(0,2);
			if (temp == 0)
			{
				auto npc = UnMovingActor::create("HPBOX", this,BloodBox, 75);
				CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
				//加载玩家坐标对象
				ValueMap  playerPointDic = objGroup->objectNamed("npc1");
				float npcX = playerPointDic.at("x").asFloat();
				float npcY = playerPointDic.at("y").asFloat();
				npc->setPosition(ccp(npcX - (1 - 2 * i) * 50, npcY - (1 - 2 * i) * 50));
				npc->setScale(0.6);
				getMap()->addChild(npc);
				allNpc.pushBack(npc);
			}
			else if (temp == 1)
			{
				auto npc = UnMovingActor::create("ManaBox", this, ManaBox, 75);
				CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
				//加载玩家坐标对象
				ValueMap  playerPointDic = objGroup->objectNamed("npc1");
				float npcX = playerPointDic.at("x").asFloat();
				float npcY = playerPointDic.at("y").asFloat();
				npc->setScale(0.6);
				npc->setPosition(ccp(npcX - (1 - 2 * i) * 50, npcY - (1 - 2 * i) * 50));
				getMap()->addChild(npc);
				allNpc.pushBack(npc);
			}
			else if (temp == 2)
			{
				auto npc = UnMovingActor::create("CoinBox", this, CoinBox, 75);
				CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
				//加载玩家坐标对象
				ValueMap  playerPointDic = objGroup->objectNamed("npc1");
				float npcX = playerPointDic.at("x").asFloat();
				float npcY = playerPointDic.at("y").asFloat();
				npc->setScale(0.6);
				npc->setPosition(ccp(npcX - (1 - 2 * i) * 50, npcY - (1 - 2 * i) * 50));
				getMap()->addChild(npc);
				allNpc.pushBack(npc);
			}
		}
	}
}

