#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Controller/MoveCtrl.h"
#include "cocos2d.h"
#include "Const/MapInfo.h"
#include"MovingActor/TestMan.h"

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	loadingAnimation();
	initMapLayer();

	initFighter();
	initListener();
	scheduleUpdate();

	/*auto rocker = MoveController::createMoveController();
	this->addChild(rocker);
	_rocker = rocker;*/



	

	/*敌人生成
	for (int i = 0; i < 4; i++)
	{
		auto soldier = Soldier::create(EAttackMode::REMOTE, ECamp::BLUE, ERoad::MIDWAY, this);
		_map->addChild(soldier);
		soldier->setPosition(200, 200 + rand() % 100 * 10);
		soldier->setScale(0.3);
		_soldiers.pushBack(soldier);
	}*/




	return true;
}
// https://blog.csdn.net/oyangyufu/article/details/26468973
void GameScene::setViewpointCenter(CCPoint position)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();  
	float x = MAX(position.x, winSize.width / 2);
	float y = MAX(position.y, winSize.height / 2);  
	x = MIN(x, (_map->getMapSize().width * _map->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_map->getMapSize().height * _map->getTileSize().height) - winSize.height / 2); 	
	CCPoint acturalPosition = ccp(x, y); 
	CCPoint centerView = ccp(winSize.width / 2, winSize.height / 2);  
	CCPoint viewPoint = ccpSub(centerView, acturalPosition);
	this->setPosition(viewPoint);
}

void GameScene::setSpritePoint(CCPoint position)
{
	CCPoint tilecoord = this->tileCoordForPosition(position);//将坐标转换成tile坐标
    CCTMXLayer* layer = _map->layerNamed("collision"); //获取地图层
	int tilegid = layer->tileGIDAt(tilecoord);//获取图素GID值
	CCLOG("tilegid: %d", tilegid);
	if (tilegid)
	{
		CCDictionary* properties = _map->propertiesForGID(tilegid);//根据GID值获取图素属性键值集合
		if (properties)
		{
			const CCString* str = properties->valueForKey("ShiTou");//键值名称
			if (str && str->compare("true") == 0) 	 //键值
			return;
		}
	}
	fighter->setPosition(position);

}
void GameScene::initMapLayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	_map = CCTMXTiledMap::create("ArtDesigning/SceneAndMap/GameMap/GameMap1/GameMap1.tmx");
	auto size = _map->getBoundingBox().size;
	_map->setAnchorPoint(Vec2(0.5, 0.5));
	_map->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	_map->setScale(0.5);

	auto collisionLayer = _map->getLayer("collision");
	collisionLayer->setVisible(false);

	addChild(_map, 0, 10000);//TAG_MAP
}
CCPoint GameScene::tileCoordForPosition(CCPoint position)
{
	int x = position.x / _map->getTileSize().width;
	int y = ((_map->getMapSize().height * _map->getTileSize().height) - position.y) / _map->getTileSize().height;
	return ccp(x, y);
}
//刷新敌人？
void GameScene::generateEnemies(float delta)
{
	if (true) //生成boss的条件
	{
		auto frame = Sprite::create("ArtDesigning/Word&Others/Comonent/CompBackground.png");
		frame->setPosition(Vec2(512, 0));
		this->addChild(frame);

		auto blood = Sprite::create("ArtDesigning/Word&Others/Comonent/YellowComp.png");

		ProgressTimer* comp = ProgressTimer::create(blood);
		comp->setType(ProgressTimer::Type::BAR);
		comp->setPosition(Vec2(0, 512));
		comp->setMidpoint(Vec2(0, 0.5));
		comp->setBarChangeRate(Vec2(1, 0));
		this->addChild(comp);
	}
}
//初始化主角,目前少角色类（在多角色可选的前提下）
void GameScene::initFighter()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	//_myHero->setTag(TAG_MYHERO);
	
	//玩家生成
	//addFighter 源码 https://blog.csdn.net/u010778159/article/details/43956151?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-2
	CCSprite* playerSprite = CCSprite::create("ArtDesigning/Sprite/Fighter/KnightDown.png");
	fighter = Testman::create(this, "Ranger");
	//fighter->bindSprite(playerSprite);
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

	updateFighterPosition();
	CCPoint sp_point = fighter->getPosition();
	setViewpointCenter(sp_point); //设置视图中心点
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
	myComp->setPercentage((((float)_myFighter->getCurHitPoints())/_myFighter->getMaxHitpoints())*100);

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
		_myFighter->fighterMove();
		_myFighter->stand();
	}
	auto newPosition = _myFighter->getPosition();
	//碰撞检测
	/*if (_mapInformation.checkCollision(newPosition))
	{
		_myFighter->fighterMove();
		if (nowTime - _myFighter->getLastAttackTime() > _myFighter->getMinAttackInterval() && _myFighter->getVertigoLastTo() < nowTime)
		{
			_map->setPosition(_map->getPosition() - positionDelta);
		}
	}
		else
		{
			_command.isHeroMove = true;
		}
	}
	else
	{
		if (_gameMode == 0)
		{
			_myHero->stopMove();
		}
		else
		{
			_command.isHeroStopMove = true;
		}
	}*/
}

void GameScene::updateFlyingItem()
{
	for (auto current = flyingItem.begin(); current != flyingItem.end();)
	{
		(*current)->fly();
		current++;
	}
}

//帧更新
void GameScene::update(float delta)
{
	updateFighterPosition();
	updateFlyingItem();



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
	auto test = dynamic_cast<Testman*>(_myFighter);
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