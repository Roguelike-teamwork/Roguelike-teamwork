#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Controller/MoveCtrl.h"
#include "cocos2d.h"
//#include "Const/MapInfo.h"
#include"MovingActor/Knight.h"
#include"MovingActor/EnemyMelee.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");

}


bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;

	}
	_gameBegin = true;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	loadingAnimation();
	initMapLayer();

	initFighter();
	initListener();
	scheduleUpdate();

	auto rocker = MoveController::createMoveController();
	this->addChild(rocker);
	_rocker = rocker;

	generateEnemies(1);


	return true;
}

// https://blog.csdn.net/oyangyufu/article/details/26468973
void GameScene::setViewpointCenter(Vec2 position)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();//有点问题：一进去的时候没有设置好位置
	float x = MAX(position.x, visibleSize.width / 2);
	float y = MAX(position.y, visibleSize.height / 2);
	x = MIN(x, (_map->getMapSize().width * _map->getTileSize().width) - visibleSize.width / 2);
	y = MIN(y, (_map->getMapSize().height * _map->getTileSize().height) - visibleSize.height / 2);
	Vec2 pointA = Vec2(visibleSize.width / 2, visibleSize.height / 2);//屏幕中心点
	Vec2 pointB = Vec2(x, y);//使精灵处于屏幕中心，移动地图目标位置；
	log("目标位置(%f,%f)", pointB.x, pointB.y);

	//地图移动偏移量
	Vec2 offset = pointA - pointB;
	offset.y += 300;
	log("offset(%f,%f)", offset.x, offset.y);
	this->setPosition(offset);
}

void GameScene::initMapLayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	_map = CCTMXTiledMap::create("ArtDesigning/SceneAndMap/GameMap/GameMap1/GameMap1.tmx");
	auto size = _map->getBoundingBox().size;
	_map->setAnchorPoint(Vec2(0,0));
	_map->setPosition(Vec2(-200, -300));
	int i = origin.x;
	_collidable = _map->getLayer("collision");
	_collidable->setVisible(false);
	_collidable2 = _map->getLayer("collision2");
	_collidable2->setVisible(false);
	addChild(_map, 0, 10000);//TAG_MAP
}

CCPoint GameScene::tileCoordForPosition(CCPoint pos)

{
	CCSize mapTiledNum = _map->getMapSize();
	CCSize tiledSize = _map->getTileSize();
	int x = pos.x / tiledSize.width;//转换为瓦块的个数横坐标 
	int y = (mapTiledNum.height* tiledSize.height - pos.y) / tiledSize.height;//转换为瓦块的个数纵坐标
	return ccp(x, y);
}


//生成敌人
void GameScene::generateEnemies(float delta)
{
	CCSprite* enemySprite = CCSprite::create("ArtDesigning/Sprite/enemy/pig/pig1.png");
	
	Enemy * enemyMelee_1 = EnemyMelee::create(this, "Ranger");
	enemyMelee_1->bindSprite(enemySprite);

	//加载对象层
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	//加载玩家坐标对象
	ValueMap  playerPointDic = objGroup->objectNamed("SpawnPoint");
	float playerX = playerPointDic.at("x").asFloat();
	float playerY = playerPointDic.at("y").asFloat();
	CCLOG("sprite postion x: %f, %f", playerX, playerY);
	enemyMelee_1->setPosition(ccp(playerX, playerY));
	_map->addChild(enemyMelee_1, 1);
	_enemies.pushBack(enemyMelee_1);
}

//初始化主角,目前少角色类（在多角色可选的前提下）
void GameScene::initFighter()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	//_myHero->setTag(TAG_MYHERO);
	
	//玩家生成
	//addFighter 源码 https://blog.csdn.net/u010778159/article/details/43956151?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-2
	CCSprite* playerSprite = CCSprite::create("ArtDesigning/Sprite/Fighter/downDir.png");
	fighter = Knight::create(this, "Ranger");
	fighter->bindSprite(playerSprite);

	//加载对象层
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	//加载玩家坐标对象
	ValueMap  playerPointDic = objGroup->objectNamed("SpawnPoint");
	float playerX = playerPointDic.at("x").asFloat();
	float playerY = playerPointDic.at("y").asFloat();
	CCLOG("sprite postion x: %f, %f", playerX, playerY);
	fighter->setPosition(ccp(playerX, playerY));
	_map->addChild(fighter, 1);
	_myFighter = fighter;
	setViewpointCenter(fighter->getPosition());
}

void GameScene::initListener()
{
	//创建键盘监听器
	listenerKeyBoard = EventListenerKeyboard::create();
	//绑定监听事件
	listenerKeyBoard->onKeyPressed = CC_CALLBACK_2(GameScene::onPressKey, this);
	listenerKeyBoard->onKeyReleased = CC_CALLBACK_2(GameScene::onReleaseKey, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyBoard, this);
}
void GameScene::initComp()
{
	auto frame = Sprite::create("ArtDesigning/Word&Others/Comonent/CompBackground.png");
	frame->setPosition(Vec2(0, 750));
	this->addChild(frame);
	auto blood = Sprite::create("ArtDesigning/Word&Others/Comonent/RedComp.png");
	ProgressTimer* comp = ProgressTimer::create(blood);
	comp->setType(ProgressTimer::Type::BAR);
	comp->setPosition(Vec2(0, 750));
	comp->setMidpoint(Vec2(0, 0.5));
	comp->setTag(FRIENDLY);
	comp->setBarChangeRate(Vec2(1, 0));
	this->addChild(comp);
}
void GameScene::updateComp()
{
	auto myComp = (ProgressTimer*)this->getChildByTag(FRIENDLY);
	myComp->setPercentage((((float)_myFighter->getCurHitPoints()) / _myFighter->getMaxHitpoints()) * 100);
	if (!enemyBoss.empty())
	{
		auto it = enemyBoss.begin();
		auto myComp = (ProgressTimer*)this->getChildByTag(ENEMY);
		myComp->setPercentage((((float)(*it)->getCurHitPoints()) / (*it)->getCurHitPoints()) * 100);
	}
}
//加载动画（一个旋涡状）
void GameScene::loadingAnimation()
{

}

//更新主角位置
void GameScene::updateFighterPosition()
{
	auto nowTime = GetCurrentTime() / 1000.f;
	if (!_myFighter->getAlreadyDead())
	{
		_myFighter->setFDirection(_rocker->getFirstDirection());
		_myFighter->setLDriection(_rocker->getLastDirection());
		_myFighter->setDirection(_rocker->getDirection());	
	}
	auto newPosition = _myFighter->updateDestination();
	CCPoint tiledpos = tileCoordForPosition(ccp(newPosition.x, newPosition.y));
	CCLOG("tiled x = %f,y = %f", tiledpos.x, tiledpos.y);
	//碰撞检测
	int tileGid = _collidable->tileGIDAt(tiledpos);
	if (tileGid > 0) 
	{
		Value prop = _map->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		std::string collision = propValueMap["collidable"].asString();

		if (collision == "true")
		{
			return;
		}
	}
	setViewpointCenter(newPosition);
	_myFighter->fighterMove(newPosition);
	_myFighter->stand();
}

void GameScene::updateEnemyPosition()
{
	auto nowTime = GetCurrentTime() / 1000.f;
	for (auto& i : _enemies)
	{
		if (!i->getAlreadyDead()&&i->getIsMoving()==false)
		{
			i->updateDestination();
    		auto newPosition = i->getDestination();
			if( !(newPosition.x >= 32 * 18 && newPosition.x <= 31 * 32 && newPosition.y >= 17 * 32 && newPosition.y <= 32 * 32))//注意瓦片地图的y方向与屏幕相反
			{
				return;
			}
			CCPoint tiledpos = tileCoordForPosition(ccp(newPosition.x, newPosition.y));
			CCLOG("tiled x = %f,y = %f", tiledpos.x, tiledpos.y);
			//碰撞检测
			int tileGid = _collidable2->tileGIDAt(tiledpos);
			if (tileGid > 0)
			{
				Value prop = _map->getPropertiesForGID(tileGid);
				ValueMap propValueMap = prop.asValueMap();

				std::string collision = propValueMap["collidable"].asString();

				if (collision == "true")
				{
					return;
				}
			}
			i->enemyMove();
		}
	}
	for (auto& i : _enemies)
	{
		if (i->getPosition() == i->getDestination())
		{
			i->setIsMoving(false);
		}
	}
}

void GameScene::updateFlyingItem()
{
	for (auto current = flyingItem.begin(); current != flyingItem.end();)
	{
		(*current)->fly();
		current++;
	}
}

//帧更新??我看不出来在哪里调用了该函数，但是确实调用了
void GameScene::update(float delta)
{
	updateFighterPosition();
	updateFlyingItem();
	updateEnemyPosition();


	/*子弹击中判断
	for (auto it = _bullets.begin(); it != _bullets.end();)
	{
		if (!(*it)->getTarget()->getAlreadyDead())
		{
			if ((*it)->calculateDistance() < (*it)->getTarget()->getBoundingBox().size.height / 2)
			{
				auto target = (*it)->getTarget();
				target->takeDamage((*it)->getDamage(), (*it)->getFromActor());
				removeChild(*it);
				it = _bullets.erase(it);
			}
			else
			{
				(*it)->calculatePosition();
				++it;
			}
		}
		else
		{
			removeChild(*it);
			it = _bullets.erase(it);
		}
	}*/

	/*敌人死亡判断
	for (auto it = _enemies.begin(); it != _enemies.end(); ++it)
	{
		if ((*it)->getAlreadyDead())
		{
			(*it)->setVisible(false);
		}
	}*/
}

void GameScene::updateEnemiesAttackTarget()
{



}

//攻击范围
void GameScene::CircleDamage(Point point, float radius, float damage)
{

}


bool GameScene::onPressKey(EventKeyboard::KeyCode keyCode, Event* event)
{
	auto test = dynamic_cast<Knight*>(_myFighter);
	if (keyCode == EventKeyboard::KeyCode::KEY_J)
	{
		test->attack();
	}
	keys[keyCode] = true;
	return true;
}


bool GameScene::onReleaseKey(EventKeyboard::KeyCode keyCode, Event* event)
{
	//keys[keyCode] = false;
	return true;
}


bool GameScene::isKeyPressed(EventKeyboard::KeyCode keyCode)
{
	if (keys[keyCode])
	{
		return true;
	}
	else
	{
		return false;
	}
}
