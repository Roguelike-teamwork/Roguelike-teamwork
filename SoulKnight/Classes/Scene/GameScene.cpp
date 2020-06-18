#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Controller/MoveCtrl.h"
#include "cocos2d.h"
#include "Enemy/Dragon.h"
#include"Enemy/Goblin.h"
#include"MovingActor/Knight.h"
#include"MovingActor/EnemyMelee.h"
#include"Weapon/Pistol.h"
#include"Weapon/Fork.h"
#include"Scene/MenuScene.h"

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
	initComp();
	initListener();
	initNextScene();
	initSettingMenu();
	initCoin();

	auto npc = UnMovingActor::create("CoinBox",this,CoinBox,50);
	npc->setPosition(_myFighter->getPosition());
	this->getMap()->addChild(npc);
	allNpc.pushBack(npc);

	auto rocker = MoveController::createMoveController();
	this->addChild(rocker);
	_rocker = rocker;

	generateEnemies(1);
	

	scheduleUpdate();
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

	//地图移动偏移量
	Vec2 offset = pointA - pointB;
	offset.y += 300;	
	this->setPosition(offset);
}

void GameScene::initMapLayer()
{
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	_map = CCTMXTiledMap::create("ArtDesigning/SceneAndMap/GameMap/GameMap1/GameMap1.tmx");
	_map->setAnchorPoint(Vec2(0, 0));
	_map->setPosition(Vec2(-200, -300));
	_collidable = _map->getLayer("collision");
	_collidable->setVisible(false);
	_collidable2 = _map->getLayer("collision2");
	_collidable2->setVisible(false);
	_fenceBool = _map->getLayer("fenceBool");
	_fenceBool->setVisible(false);
	_collisionForFight = _map->getLayer("collisionForFight");
	_collisionForFight->setVisible(false);
	addChild(_map, 0, 10000);//TAG_MAP
}
void GameScene::initSettingMenu()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。

	menuMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/MENU.png",
		"ArtDesigning/Word&Others/button/MENU.png",
		CC_CALLBACK_1(GameScene::menuMenuCallBack, this)
	);

	if (menuMenu == nullptr ||
		menuMenu->getContentSize().width <= 0 ||
		menuMenu->getContentSize().height <= 0)
	{
		problemLoading("'menuMenu .png'");
	}
	else
	{
		float x = origin.x + visibleSize.width;
		float y = origin.y + visibleSize.height;
		menuMenu->setPosition(Vec2(x - 30, y - 30));
	}

	//mu
	Menu* mu = Menu::create(menuMenu, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 1);

}
void GameScene::menuMenuCallBack(cocos2d::Ref* pSender)
{
	//转到菜单项(可返回)
	auto nextScene = MenuScene::create();
	Director::getInstance()->pushScene(nextScene);
	MenuItem* item = static_cast<MenuItem*>(pSender);


	log("Touch Menu Menu Item %p", item);
}
void GameScene::menuPlayCallBack(cocos2d::Ref* pSender)
{
	//开始游戏
	auto nextScene = GameScene::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = static_cast<MenuItem*>(pSender);

	log("Touch Transfer Item %p", item);
}
void GameScene::initNextScene()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();//得到屏幕大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//获得可视区域的出发点坐标，在处理相对位置时，确保节点在不同分辨率下的位置一致。

	//加载对象层
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	//加载玩家坐标对象
	ValueMap  playerPointDic = objGroup->objectNamed("SpawnPoint");
	float playerX = playerPointDic.at("x").asFloat();
	float playerY = playerPointDic.at("y").asFloat();
	CCLOG("sprite postion x: %f, %f", playerX, playerY);

	//playMenu
	MenuItemImage* playMenu = MenuItemImage::create(
		"ArtDesigning/Word&Others/button/PLAY.png",
		"ArtDesigning/Word&Others/button/PLAY.png",
		CC_CALLBACK_1(GameScene::menuPlayCallBack, this)
	);

	if (playMenu == nullptr ||
		playMenu->getContentSize().width <= 0 ||
		playMenu->getContentSize().height <= 0)
	{
		problemLoading("'playMenu.png'");
	}
	else
	{
		playMenu->setPosition(Vec2(playerX, playerY));
	}
	//mu
	Menu* mu = Menu::create(playMenu, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 1);

}
void GameScene::initCoin()
{  
	coinNum = 0;
	coin = Label::createWithBMFont("fonts/Curlz.fnt", "0");//Label::createWithBMFont表示添加图片字体，”helvetica-32.fnt”为图片字体的格式，”Score:0”表示在屏幕上显示的得分信息 
	this->addChild(coin, 2);

	coin->setPosition(70, 70);
	coin->setScale(2);
	coin->setTag(TAG_layer_Score);
	coinPicture = Sprite::create("ArtDesigning/Word&Others/others/coin.png");
	this->addChild(coinPicture, 2);
	coinPicture->setPosition(coin->getPositionX() - 35, coin->getPositionY() - 5);
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
	Enemy * enemyMelee_1 = Dragon::create(this, "Dragon");

	//加载对象层
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	//加载玩家坐标对象
	ValueMap  playerPointDic = objGroup->objectNamed("SpawnPoint");
	float playerX = playerPointDic.at("x").asFloat();
	float playerY = playerPointDic.at("y").asFloat();

	enemyMelee_1->setPosition(ccp(playerX, playerY));
	_map->addChild(enemyMelee_1, 1);
	enemyBoss.pushBack(enemyMelee_1);
	allEnemy.pushBack(enemyMelee_1);
}

//初始化主角,目前少角色类（在多角色可选的前提下）
void GameScene::initFighter()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	//_myHero->setTag(TAG_MYHERO);
	
	//玩家生成
	//addFighter 源码 https://blog.csdn.net/u010778159/article/details/43956151?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-2
	//CCSprite* playerSprite = CCSprite::create("ArtDesigning/Sprite/Fighter/KnightDown.png");
	auto fighter = Knight::create(this, "Knight");
	//fighter->bindSprite(playerSprite);
	allFighter.pushBack(fighter);

	//加载对象层
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	//加载玩家坐标对象
	ValueMap  playerPointDic = objGroup->objectNamed("SpawnPoint");
	float playerX = playerPointDic.at("x").asFloat();
	float playerY = playerPointDic.at("y").asFloat();

	fighter->setPosition(ccp(playerX, playerY));
	_map->addChild(fighter, 1);
	_myFighter = fighter;
	setViewpointCenter(fighter->getPosition());

//是

	//生成角色对应的武器
	auto weapon = Pistol::create(EAttackMode::REMOTE,"Pistol",7,0.2,500,1);
	weapon->setPosition(_myFighter->getPosition());
	this->getMap()->addChild(weapon);
	allWeapon.pushBack(weapon);

	auto weapon_1 = Fork::create(EAttackMode::MELEE,"Fork",4,0.5f,100,2);
	weapon_1->setPosition(Vec2(_myFighter->getPosition().x+100,_myFighter->getPosition().y+100));
	this->getMap()->addChild(weapon_1);
	allWeapon.pushBack(weapon_1);
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
	frame->setPosition(_myFighter->getPosition().x, _myFighter->getPosition().y + 50);
	this->addChild(frame);
	auto blood = Sprite::create("ArtDesigning/Word&Others/Comonent/RedComp.png");
	ProgressTimer* comp = ProgressTimer::create(blood);
	comp->setType(ProgressTimer::Type::BAR);
	comp->setPosition(_myFighter->getPosition().x,_myFighter->getPosition().y+50);
	comp->setMidpoint(Vec2(0, 0.5));
	comp->setTag(FRIENDLY);
	comp->setBarChangeRate(Vec2(1, 0));
	this->addChild(comp);
}
void GameScene::updateComp()
{
	auto myComp = (ProgressTimer*)this->getChildByTag(FRIENDLY);
	myComp->setPercentage((((float)_myFighter->getCurHitPoints()) / _myFighter->getHitpoints()) * 100);

}
//加载动画（一个旋涡状）
void GameScene::loadingAnimation()
{

}

//更新主角位置
void GameScene::updateFighterPosition()
{
	auto nowTime = GetCurrentTime() / 1000.f;


	if (_myFighter->getAlreadyDead())
		return;

	auto it = dynamic_cast<Knight*>(_myFighter);
	it->updateSkill();

	if (it->getCanAttack())
		it->attack();
	
	_myFighter->updateCondition();
	_myFighter->setIsMoving(_rocker->getIsCanMove());
	_myFighter->setFDirection(_rocker->getFirstDirection());
	_myFighter->setDirection(_rocker->getDirection());
	_myFighter->setLDriection(_rocker->getLastDirection());
	
	auto newPosition = _myFighter->updateDestination();
	CCPoint tiledpos = tileCoordForPosition(ccp(newPosition.x, newPosition.y));

	
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
	if (collisionForFightBool)
	{
		tileGid = _collisionForFight->tileGIDAt(tiledpos);
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
	}
	tileGid = _fenceBool->tileGIDAt(tiledpos);
	if (tileGid > 0)
	{
		Value prop = _map->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		std::string fenceBool = propValueMap["collidable"].asString();

		if (fenceBool == "true")
		{
			collisionForFightBool = true;
		}
	}
	
	getMap()->setPosition(this->getMap()->getPosition().x+_myFighter->getPosition().x-newPosition.x,
						  this->getMap()->getPosition().y+_myFighter->getPosition().y-newPosition.y);

	

	if(_myFighter->getIsMoving())
		_myFighter->fighterMove(newPosition);
	else
		_myFighter->stand();
}

void GameScene::updateNPC()
{
	for (auto& it : allNpc)
	{
		if (it->isInRadius(_myFighter))
		{
			it->effect(_myFighter);
			currentNPC = it;
			break;
		}
		currentNPC = NULL;
	}
}

void GameScene::updateEnemyPosition()
{
	auto nowTime = GetCurrentTime() / 1000.f;
	for (auto& i : allEnemy)
	{
		if(i->getLevel()==EnemyLevel::SOLDIER)
			i->updateAction();
		else if(i->getLevel()==EnemyLevel::BOSS)
		{
			auto it = dynamic_cast<Dragon*>(i);
			it->updateState();
		}
		if (!i->getAlreadyDead()&&i->getIsMoving()==false)
		{
			i->updateDestination();
    		auto newPosition = i->getDestination();
			if( !(newPosition.x >= 32 * 18 && newPosition.x <= 31 * 32 && newPosition.y >= 17 * 32 && newPosition.y <= 32 * 32))//注意瓦片地图的y方向与屏幕相反
			{
				return;
			}
			CCPoint tiledpos = tileCoordForPosition(ccp(newPosition.x, newPosition.y));
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
	for (auto& i : enemySoldier)
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

	for (auto current = flyingItem.begin(); current != flyingItem.end();current++)
	{
		//假如子弹的发出者死亡，子弹将被消除
		if ((*current)->getOwner()==nullptr)
		{
			(*current)->setIsToClean(true);
			continue;
		}


		if ((*current)->getOwner()->getCamp() == FRIENDLY)
		{
			if (!enemySoldier.empty())
			{
				for (auto& it : enemySoldier)
				{
					if (it->getPosition().getDistance((*current)->getPosition()) < 10)
					{
						it->takeDamage(DamageMode::COMMON, (*current)->getDamage(), (*current)->getOwner());
						(*current)->setIsToClean(true);
						break;
					}
				}
			}
			if (!enemyBoss.empty())
			{
				for (auto& it : enemyBoss)
				{
					if (it->getPosition().getDistance((*current)->getPosition()) < 10)
					{
						it->takeDamage(DamageMode::COMMON, (*current)->getDamage(), (*current)->getOwner());
						(*current)->setIsToClean(true);
						break;
					}
				}
			}
		}
		if ((*current)->getOwner()->getCamp() == ENEMY)
		{
			//因为只有一个主角就这么写了
			if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 10)
			{
				if (_myFighter->getCanBeHurt())
				{
					_myFighter->setLastTimeInjured(GetCurrentTime() / 1000.f);
					_myFighter->hurt((*current)->getDamage());
					(*current)->setIsToClean(true);
				}
			}
		}
	}
}

void GameScene::updateSpecialBullet()
{
	auto nowTime = GetCurrentTime()/1000.f;
	for (auto current = specialBullet.begin(); current != specialBullet.end(); current++)
	{
		//对Boss龙的第三技能产生的“子弹”进行处理
		if ((*current)->getOwner()->getCamp() == AllCamp::FRIENDLY)
		{
			//Knight仅设计了一把近战武器--叉子
			if (!enemySoldier.empty())
			{
				for (auto& it : enemySoldier)
				{
					if (it->getPosition().getDistance((*current)->getPosition()) < 40)
					{
						it->takeDamage(DamageMode::COMMON, (*current)->getDamage(), (*current)->getOwner());
					}
				}
			}
			if (!enemyBoss.empty())
			{
				for (auto& it : enemyBoss)
				{
					if (it->getPosition().getDistance((*current)->getPosition()) < 40)
					{
						it->takeDamage(DamageMode::COMMON, (*current)->getDamage(), (*current)->getOwner());
					}
				}
			}

			if (nowTime - (*current)->getGiveOutTime() > 1.0/15.0f)
				(*current)->setIsToClean(true);
		}
		else
		{
			auto it = dynamic_cast<Enemy*>((*current)->getOwner());
			if (it->getLevel() == BOSS)
			{
				if (nowTime - (*current)->getGiveOutTime() > 1.0f)
				{
					if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 10 && _myFighter->getCanBeHurt())
					{
						_myFighter->hurt((*current)->getDamage());
						_myFighter->setLastTimeInjured(GetCurrentTime()/1000.f);
					}
				}

				if (nowTime - (*current)->getGiveOutTime() > 2.0f)
					(*current)->setIsToClean(true);
			}
			else
			{
				if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 10 && _myFighter->getCanBeHurt())
				{
					_myFighter->hurt((*current)->getDamage());
					_myFighter->setLastTimeInjured(GetCurrentTime() / 1000.f);
				}
				if (nowTime - (*current)->getGiveOutTime() > 0.3f)
					(*current)->setIsToClean(true);
			}
		}
	}
}

void GameScene::clearObject()
{
	for (auto it = allWeapon.begin(); it != allWeapon.end();)
	{
		if ((*it)->getNowState() == WeaponStatus::TAKEN)
		{
			this->getMap()->removeChild(*it);
			it = allWeapon.erase(it);
		}
		else
		{
			++it;
		}
	}
	for (auto it = flyingItem.begin(); it != flyingItem.end();)
	{
		if ((*it)->getIsToClean()||(*it)->getOwner()->getAlreadyDead())
		{
			this->getMap()->removeChild(*it);
			it = flyingItem.erase(it);
		}
		else
			++it;
	}

	for (auto it = specialBullet.begin(); it != specialBullet.end();)
	{
		if ((*it)->getIsToClean() || (*it)->getOwner()->getAlreadyDead())
		{
			this->getMap()->removeChild(*it);
			it = specialBullet.erase(it);
		}
		else
			++it;
	}

	for (auto it = allEnemy.begin(); it != allEnemy.end();)
	{
		if ((*it)->getAlreadyDead())
		{
			this->getMap()->removeChild(*it);
			it = allEnemy.erase(it);
		}
		else
			++it;
	}
	for (auto it = enemySoldier.begin(); it != enemySoldier.end();)
	{
		if ((*it)->getAlreadyDead())
		{
			this->getMap()->removeChild(*it);
			it = enemySoldier.erase(it);
		}
		else
			++it;
	}
	for (auto it = enemyBoss.begin(); it != enemyBoss.end();)
	{
		if ((*it)->getAlreadyDead())
		{
			this->getMap()->removeChild(*it);
			it = enemyBoss.erase(it);
		}
		else
			++it;
	}
}

//帧更新??我看不出来在哪里调用了该函数，但是确实调用了
void GameScene::update(float delta)
{
	updateFighterPosition();
	updateFlyingItem();
	updateSpecialBullet();
	updateEnemyPosition();
	updateComp();
	updateNPC();
	clearObject();


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

void GameScene::updateWeapon()
{
	for (auto& it : allWeapon)
	{
		auto distance = _myFighter->getPosition().getDistance(it->getPosition());
		if (distance < 40)
		{
			tempWeapon = it;
			it->setIsCanPick(true);
			return;
		}
		else
			it->setIsCanPick(false);
	}
	tempWeapon = NULL;
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
		test->setCanAttack(true);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_L)
	{
		updateWeapon();
		for (auto& it : allWeapon)
		{
			if (it->getIsCanPick())
			{
				test->getWeapon(it);
				it->setOwner(test);
				it->setNowState(WeaponStatus::TAKEN);
			}
		}
		if (currentNPC!=nullptr)
			currentNPC->setIsConfirm(true);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_K)
	{
		test->setCurrentWeapon(test->changeMainEquip());
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_U)
	{
		if (test->getCanRelease())
		{
			test->releaseSkill();
		}
	}

	keys[keyCode] = true;
	return true;
}


bool GameScene::onReleaseKey(EventKeyboard::KeyCode keyCode, Event* event)
{
	//keys[keyCode] = false;
	auto it = dynamic_cast<Knight*>(_myFighter);
	if (keyCode == EventKeyboard::KeyCode::KEY_J)
		it->setCanAttack(false);
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