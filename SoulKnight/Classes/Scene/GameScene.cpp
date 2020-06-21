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
#include"Scene/MenuScene.h"
#include"Scene/Stage_2.h"
#include"Weapon/Sword.h"
#include"Weapon/Dart.h"
#include"Scene/testmanSelectScene.h"
#include"Scene/DieScene.h"
#include"Scene/SafetyMapScene.h"

USING_NS_CC;

Scene* GameScene::createScene(testmanSelect* scene)
{
	return GameScene::create(scene);
}


GameScene* GameScene::create(testmanSelect* scene)
{
	GameScene* newScene = new GameScene();
	if (newScene && newScene->init(scene))
	{
		newScene->autorelease();
		return newScene;
	}
	CC_SAFE_DELETE(newScene);
	return NULL;
}
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");

}

bool GameScene::init(testmanSelect* scene)
{
	if (!Scene::init())
	{
		return false;

	}
	_gameBegin = true;
	setPtestmanSelect(scene);
	updateGrade();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	return true;
}

void GameScene::updateGrade()
{
	auto grade = getPtestmanSelect()->getGrade();//获得了上个scene的属性
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
	offset.y += 350;
	offset.x += 270;
	this->setPosition(offset);
}



void GameScene::initController()
{
	auto rocker = MoveController::createMoveController();
	this->addChild(rocker);
	_rocker = rocker;
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


//初始化主角,目前少角色类（在多角色可选的前提下）
void GameScene::initFighter()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	//_myHero->setTag(TAG_MYHERO);
	
	//玩家生成
	//addFighter 源码 https://blog.csdn.net/u010778159/article/details/43956151?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-2
	
	auto fighter = Knight::create(this, "Knight");
	allFighter.pushBack(fighter);
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	ValueMap  playerPointDic = objGroup->objectNamed("SpawnPoint");
	float playerX = playerPointDic.at("x").asFloat();
	float playerY = playerPointDic.at("y").asFloat();

	fighter->setPosition(ccp(playerX, playerY));
	_map->addChild(fighter, 1);
	_myFighter = fighter;
	setViewpointCenter(fighter->getPosition());


	//生成角色对应的武器
	auto weapon = Pistol::create(EAttackMode::REMOTE,"Pistol",PISTOL_DAMAGE,PISTOL_ATTACKSPEED,500,PISTOL_MPCONSUME);
	weapon->setPosition(Vec2(_myFighter->getPosition().x-50, _myFighter->getPosition().y));
	this->getMap()->addChild(weapon);
	allWeapon.pushBack(weapon);

	auto weapon_1 = Sword::create(EAttackMode::MELEE,"Sword",SWORD_DAMAGE,SWORD_ATTACKSPEED,SWORD_ATTACKR,SWORD_MPCONSUME);
	weapon_1->setPosition(Vec2(_myFighter->getPosition().x,_myFighter->getPosition().y-50));
	this->getMap()->addChild(weapon_1);
	allWeapon.pushBack(weapon_1);

	auto weapon_2 = Fork::create(EAttackMode::MELEE, "Fork",FORK_DAMAGE,FORK_ATTACKSPEED, FORK_ATTACKR, FORK_MPCONSUME);
	weapon_2->setPosition(Vec2(_myFighter->getPosition().x, _myFighter->getPosition().y + 50));
	this->getMap()->addChild(weapon_2);
	allWeapon.pushBack(weapon_2);

	auto weapon_3 = Dart::create(EAttackMode::REMOTE, "Dart", DART_DAMAGE, DART_ATTACKSPEED,500, DART_MPCONSUME);
	weapon_3->setPosition(Vec2(_myFighter->getPosition().x+50, _myFighter->getPosition().y));
	this->getMap()->addChild(weapon_3);
	allWeapon.pushBack(weapon_3);

}
void GameScene::initUI()
{
	auto burnSprite = Sprite::create("ArtDesigning/Word&Others/others/Burn.png");
	burnSprite->setPosition(70,550);
	burnSprite->setVisible(false);
	burnSprite->setTag(682);
	allUI.pushBack(burnSprite);
	this->addChild(burnSprite);



	auto poisonSprite = Sprite::create("ArtDesigning/Word&Others/others/Poison.png");
	poisonSprite->setPosition(70, 550);
	poisonSprite->setVisible(false);
	poisonSprite->setTag(683);
	allUI.pushBack(poisonSprite);
	this->addChild(poisonSprite);

	auto slowSprite = Sprite::create("ArtDesigning/Word&Others/others/SpeedDown.png");
	slowSprite->setPosition(70, 550);
	slowSprite->setVisible(false);
	slowSprite->setTag(695);
	allUI.pushBack(slowSprite);
	this->addChild(slowSprite);

	auto pistolSprite = Sprite::create("ArtDesigning/FlyingItem/Weapon/Pistol.png");
	pistolSprite->setPosition(70, 480);
	pistolSprite->setVisible(false);
	pistolSprite->setTag(684);
	allUI.pushBack(pistolSprite);
	this->addChild(pistolSprite);

	auto forkSprite = Sprite::create("ArtDesigning/FlyingItem/Weapon/Fork.png");
	forkSprite->setPosition(70, 480);
	forkSprite->setVisible(false);
	forkSprite->setTag(685);
	allUI.pushBack(forkSprite);
	this->addChild(forkSprite);

	auto swordSprite = Sprite::create("ArtDesigning/FlyingItem/Weapon/Sword.png");
	swordSprite->setPosition(70, 480);
	swordSprite->setVisible(false);
	swordSprite->setTag(686);
	allUI.pushBack(swordSprite);
	this->addChild(swordSprite);

	auto dartSprite = Sprite::create("ArtDesigning/FlyingItem/Weapon/Dart.png");
	dartSprite->setPosition(70, 480);
	dartSprite->setVisible(false);
	dartSprite->setTag(687);
	allUI.pushBack(dartSprite);
	this->addChild(dartSprite);

	auto skillSprite = Sprite::create("ArtDesigning/Word&Others/others/Skill.png");
	skillSprite->setPosition(70, 430);
	skillSprite->setVisible(false);
	skillSprite->setTag(688);
	allUI.pushBack(skillSprite);
	this->addChild(skillSprite);

	auto skillMessage = Label::create("","Marker Felt",30);
	skillMessage->setPosition(70, 430);
	skillMessage->setTag(689);
	this->addChild(skillMessage,10);

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
	auto frame = Sprite::create("ArtDesigning/Word&Others/others/CompBackground.png");
	frame->setPosition(70,698);
	frame->setScale(2);
	this->addChild(frame);

	auto frame_1 = Sprite::create("ArtDesigning/Word&Others/others/CompBackground.png");
	frame_1->setPosition(70, 658);
	frame_1->setScale(2);
	this->addChild(frame_1);

	auto frame_2 = Sprite::create("ArtDesigning/Word&Others/others/CompBackground.png");
	frame_2->setPosition(70, 618);
	frame_2->setScale(2);
	this->addChild(frame_2);

	auto blood = Sprite::create("ArtDesigning/Word&Others/others/RedComp.png");
	ProgressTimer* comp = ProgressTimer::create(blood);
	comp->setType(ProgressTimer::Type::BAR);
	comp->setPosition(70,698);
	comp->setMidpoint(Vec2(0, 0.5));
	comp->setScale(2);
	comp->setTag(100886);
	comp->setBarChangeRate(Vec2(1, 0));
	this->addChild(comp,5);

	auto mana = Sprite::create("ArtDesigning/Word&Others/others/ManaComp.png");
	ProgressTimer* comp_1 = ProgressTimer::create(mana);
	comp_1->setType(ProgressTimer::Type::BAR);
	comp_1->setPosition(70, 658);
	comp_1->setMidpoint(Vec2(0, 0.5));
	comp_1->setScale(2);
	comp_1->setTag(100887);
	comp_1->setBarChangeRate(Vec2(1, 0));
	this->addChild(comp_1,5);

	auto theShield = Sprite::create("ArtDesigning/Word&Others/others/ShieldComp.png");
	ProgressTimer* comp_2 = ProgressTimer::create(theShield);
	comp_2->setType(ProgressTimer::Type::BAR);
	comp_2->setPosition(70, 618);
	comp_2->setMidpoint(Vec2(0, 0.5));
	comp_2->setScale(2);
	comp_2->setTag(100888);
	comp_2->setBarChangeRate(Vec2(1, 0));
	this->addChild(comp_2,5);
}
void GameScene::updateComp()
{
	auto myComp_HP = (ProgressTimer*)(this->getChildByTag(100886));
	myComp_HP->setPercentage((((float)_myFighter->getCurHitPoints()) / _myFighter->getHitpoints()) * 100);

	auto myComp_MP = (ProgressTimer*)(this->getChildByTag(100887));
	myComp_MP->setPercentage((((float)_myFighter->getCurManaPoints()) / _myFighter->getManaPoints()) * 100);

	auto myComp_SP = (ProgressTimer*)(this->getChildByTag(100888));
	myComp_SP->setPercentage((((float)_myFighter->getCurShield()) / _myFighter->getShield()) * 100);


	for (auto& i : allComp)
	{
		if(i->getOwner()!=nullptr)
				i->updateState();
	}
}

void GameScene::generateComp()
{
	for (auto& i : allEnemy)
	{
		auto temp = Comp::create(i);
		allComp.pushBack(temp);
	}
}


//加载动画（一个旋涡状）
void GameScene::loadingAnimation()
{
	updateMusic();
	//上方向动画

	auto temp_1 = Animation::create();
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Fighter/%sUp_%d.png", _myFighter->getFighterName().getCString(), i);
		temp_1->addSpriteFrameWithFileName(frameName->getCString());
	}
	AnimationCache::getInstance()->addAnimation(temp_1,String::createWithFormat("%sUp",_myFighter->getFighterName().getCString())->getCString());

	auto temp_2 = Animation::create();
	//左上
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Fighter/%sUpLeft_%d.png", _myFighter->getFighterName().getCString(), i);
		temp_2->addSpriteFrameWithFileName(frameName->getCString());

	}
	AnimationCache::getInstance()->addAnimation(temp_2, String::createWithFormat("%sUpLeft", _myFighter->getFighterName().getCString())->getCString());

	auto temp_3 = Animation::create();
	//右上
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Fighter/%sUpRight_%d.png", _myFighter->getFighterName().getCString(), i);
		temp_3->addSpriteFrameWithFileName(frameName->getCString());
	}
	AnimationCache::getInstance()->addAnimation(temp_3, String::createWithFormat("%sUpRight", _myFighter->getFighterName().getCString())->getCString());

	auto temp_4 = Animation::create();
	//右
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Fighter/%sRight_%d.png", _myFighter->getFighterName().getCString(), i);
		temp_4->addSpriteFrameWithFileName(frameName->getCString());
	}
	AnimationCache::getInstance()->addAnimation(temp_4, String::createWithFormat("%sRight", _myFighter->getFighterName().getCString())->getCString());


	auto temp_5 = Animation::create();
	//右下
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Fighter/%sDownRight_%d.png", _myFighter->getFighterName().getCString(), i);
		temp_5->addSpriteFrameWithFileName(frameName->getCString());
	}
	AnimationCache::getInstance()->addAnimation(temp_5, String::createWithFormat("%sDownRight", _myFighter->getFighterName().getCString())->getCString());


	auto temp_6 = Animation::create();
	//下
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Fighter/%sDown_%d.png", _myFighter->getFighterName().getCString(), i);
		temp_6->addSpriteFrameWithFileName(frameName->getCString());
	}
	AnimationCache::getInstance()->addAnimation(temp_6, String::createWithFormat("%sDown", _myFighter->getFighterName().getCString())->getCString());


	auto temp_7 = Animation::create();
	//左下
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Fighter/%sDownLeft_%d.png", _myFighter->getFighterName().getCString(), i);
		temp_7->addSpriteFrameWithFileName(frameName->getCString());
	}
	AnimationCache::getInstance()->addAnimation(temp_7, String::createWithFormat("%sDownLeft", _myFighter->getFighterName().getCString())->getCString());

	auto temp_8 = Animation::create();
	//左
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/Sprite/Fighter/%sLeft_%d.png", _myFighter->getFighterName().getCString(), i);
		temp_8->addSpriteFrameWithFileName(frameName->getCString());
	}
	AnimationCache::getInstance()->addAnimation(temp_8, String::createWithFormat("%sLeft", _myFighter->getFighterName().getCString())->getCString());

	auto temp_9 = Animation::create();
	for (int i = 1; i <= 2; i++)
	{
		auto frameName = String::createWithFormat("ArtDesigning/FlyingItem/Bullet/SwordEffect_%d.png", i);
		temp_9->addSpriteFrameWithFileName(frameName->getCString());
	}
	AnimationCache::getInstance()->addAnimation(temp_9,"Sword");



	auto sprite = Sprite::create();
	auto animation = CCAnimation::create();
	for (int i = 1; i < 4; i++)
	{
		char namesize[80] = { 0 };
		sprintf(namesize, "ArtDesigning/Word&Others/others/xuanwo%d.png", i);
		animation->addSpriteFrameWithFile(namesize);
	}
	animation->setDelayPerUnit(1.0f / 3.0f);   //1.0秒内3帧
	animation->setRestoreOriginalFrame(true);
	animation->setLoops(-1);
	auto animate = CCAnimate::create(animation);
	sprite->setScaleY(0.4);
	if (roomState == SPAWNROOM_1)
		sprite->setPosition(23 * 32, 57 * 32);
	else
		sprite->setPosition(71*32,52*32);
	getMap()->addChild(sprite, 7);
	sprite->runAction(CCRepeatForever::create(animate));



	auto poison = Sprite::create();
	auto poisonAnimation = CCAnimation::create();
	for (int i = 1; i < 11; i++)
	{
		char namesizep[100] = { 0 };
		sprintf(namesizep, "ArtDesigning/Word&Others/others/70%d.png", i);
		poisonAnimation->addSpriteFrameWithFile(namesizep);
	}
	poisonAnimation->setDelayPerUnit(3.0f / 11.0f);
	poisonAnimation->setRestoreOriginalFrame(true);
	poisonAnimation->setLoops(-1);
	auto posionAnimate = CCAnimate::create(poisonAnimation);
	poison->setScale(0.7);
	poison->setTag(10085);
	poison->setPosition(48*32,25*32);
	item.pushBack(poison);
	getMap()->addChild(poison, 4);

	
	//正反毒气动画
	auto tempAction_0 = CCRepeatForever::create(posionAnimate);
	poison->runAction(tempAction_0);
}

void GameScene::updateItem()
{
	for (auto it=item.begin();it!=item.end();)
	{
		if (_myFighter->getPosition().getDistance((*it)->getPosition()) < 50)
		{
			if ((*it)->getTag() == 10085)
				_myFighter->takeBuff(Buff::create(POISON, 0, 0, 0, 4));
			if ((*it)->getTag() == 6618)
			{
				_myFighter->takeBuff(Buff::create(NORMAL, 0, 50, 0, 5000));
				getMap()->removeChild(*it);
				it = item.erase(it);
				continue;
			}
			if ((*it)->getTag() == 6619)
			{
				_myFighter->takeBuff(Buff::create(NORMAL, 50, 0, 0, 5000));
				getMap()->removeChild(*it);
				it = item.erase(it);
				continue;
			}
		}
		it++;
	}
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
	_myFighter->setOldDirection(_myFighter->getDirection());
	_myFighter->setIsMoving(_rocker->getIsCanMove());
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
			return;
	}
	if (roomState == ROOM_1_1)
	{
		tileGid = _spikeweed->tileGIDAt(tiledpos);
		if (tileGid > 0)
		{
			if (_myFighter->getState() == NORMAL)
				_myFighter->takeBuff(Buff::create(SPEEDDOWN, 0, 0, 0, 3));
			if (_myFighter->getCanBeHurt())
				_myFighter->hurt(5);

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
				return;
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
			if(roomState==SPAWNROOM_1)
				_fence->setVisible(true);
			if (roomState == SPAWNROOM_1)
				roomState = ROOM_1_1;
			else if (roomState == SPAWNROOM_2)
				roomState = ROOM_2_1;
			else if (roomState == ROOM_2_2)
			{
				roomState = ROOM_2_3;
				updateMusic();
			}
		}
	}
	if (roomState == SPAWNROOM_1 || roomState == ROOM_1_1||roomState==ROOM_2_3)
	{
		tileGid = _transfer->tileGIDAt(tiledpos);
		if (tileGid > 0)
		{
			Value prop = _map->getPropertiesForGID(tileGid);
			ValueMap propValueMap = prop.asValueMap();

			std::string transfer = propValueMap["collidable"].asString();

			if (transfer == "true")
			{
				unscheduleUpdate();
				_eventDispatcher->removeAllEventListeners();
				if(roomState==ROOM_1_1)
					scheduleOnce(schedule_selector(GameScene::toNextScene), 3.0f);
				else
					endGame();
			}
		}
	}
	getMap()->setPosition(this->getMap()->getPosition().x+_myFighter->getPosition().x-newPosition.x,
						  this->getMap()->getPosition().y+_myFighter->getPosition().y-newPosition.y);

	

	if (_myFighter->getIsMoving())
	{
		_myFighter->fighterMove(newPosition);
		if (_myFighter->getOldDirection()!=_myFighter->getDirection())
		{
			_myFighter->playAnimation();
		}
	}
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

void GameScene::updateUI()
{
	coin->setString(String::createWithFormat("%d",coinNum)->getCString());

	for (auto& it : allUI)
	{
		int tempTag = 0;
		if (_myFighter->getState() == BURN && it->getTag() == 682)
		{
			it->setVisible(true);
			tempTag = it->getTag();
		}
		if (_myFighter->getState() == POISON && it->getTag() == 683)
		{
			it->setVisible(true);
			tempTag = it->getTag();
		}
		if (_myFighter->getState() == SPEEDDOWN && it->getTag() == 695)
		{
			it->setVisible(true);
			tempTag = it->getTag();
		}
		if ((it->getTag() == 682 || it->getTag() == 683||it->getTag()==695)&&it->getTag()!=tempTag)
			it->setVisible(false);
	}

	for (auto& it : allUI)
	{
		int tempTag = 0;
		if (!_myFighter->getCurrentWeapon())
			break;
		if (_myFighter->getCurrentWeapon()->getEquipName()==EEQUIPMENT::PISTOL && it->getTag() == 684)
		{
			it->setVisible(true);
			tempTag = it->getTag();
		}
		if (_myFighter->getCurrentWeapon()->getEquipName()==EEQUIPMENT::FORK && it->getTag() == 685)
		{
			it->setVisible(true);
			tempTag = it->getTag();
		}
		if (_myFighter->getCurrentWeapon()->getEquipName() == EEQUIPMENT::SWORD && it->getTag() == 686)
		{
			it->setVisible(true);
			tempTag = it->getTag();
		}
		if (_myFighter->getCurrentWeapon()->getEquipName() == EEQUIPMENT::DART && it->getTag() == 687)
		{
			it->setVisible(true);
			tempTag = it->getTag();
		}
		if (it->getTag() != 682 && it->getTag() != 683&&it->getTag()!=695&&it->getTag()!=tempTag)
			it->setVisible(false);
	}

	auto label = dynamic_cast<Label*>(getChildByTag(689));
	for (auto& it : allUI)
	{
		auto temp = dynamic_cast<Knight*>(_myFighter);
		if (temp->getIsRelease())
		{
			if(it->getTag()==688)
				it->setVisible(true);
			label->setVisible(false);
		}
		else
		{
			if (it->getTag() == 688)
				it->setVisible(false);
			label->setVisible(true);
		}
		
		
		if (!temp->getIsRelease())
		{
			if (it->getTag() == 688)
				it->setVisible(false);

			auto nowTime = GetCurrentTime() / 1000.f;
			if (!temp->getCanRelease())
				label->setString(String::createWithFormat("%.2f", temp->getSkillCDTime() + temp->getLastReleaseTime() - nowTime)->getCString());
			else
				label->setString("OK");

		}
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
		if (!i->getAlreadyDead()&&i->getIsMoving()==false&&i->getIsToMove())
		{
			i->updateDestination();
    		auto newPosition = i->getDestination();
		if (!islegal(newPosition.x,newPosition.y,i->getEnemyState()))
				return;
			i->enemyMove();
		}
	}
	for (auto& i : allEnemy)
	{
		if (i->getDestination().getDistance(getPosition())<i->getAttackRadius()||i->getPosition()==i->getDestination())
		{
			i->setIsMoving(false);
			i->setOldDestination(i->getDestination());
		}
	}

	for (auto& i : allEnemy)
	{
		if (i->getEnemyState()==roomState)
		{
			return;
		}
	}
	if (roomState == ROOM_2_1)
		roomState = ROOM_2_2;
	if(roomState==ROOM_1_1)
		_fence->setVisible(false);
	collisionForFightBool = false;
}

void GameScene::updateFlyingItem()
{



	for (auto current = flyingItem.begin(); current != flyingItem.end();)
	{
		(*current)->fly();
		if ((*current)->getOwner()->getCamp() == ENEMY)
		{
			auto temp = dynamic_cast<Enemy*>((*current)->getOwner());
			if (temp->getLevel() == BOSS)
			{
				current++;
				continue;
			}
		}
		CCPoint tiledpos = tileCoordForPosition((*current)->getPosition());
		//碰撞检测
		int tileGid = _collidable->tileGIDAt(tiledpos);
		if (tileGid > 0)
		{
			Value prop = _map->getPropertiesForGID(tileGid);
			ValueMap propValueMap = prop.asValueMap();

			std::string collision = propValueMap["collidable"].asString();

			//这一步的目的是为了在第二关时boss的子弹能穿过墙体对玩家进行干扰

				if (collision == "true")
				{
					(*current)->setIsToClean(true);
					current++;
					continue;
				}
			
		}
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
					if (it->getPosition().getDistance((*current)->getPosition()) < 15)
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
					if (it->getPosition().getDistance((*current)->getPosition()) < 15)
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
			if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 15)
			{
				if (_myFighter->getCanBeHurt())
				{
					_myFighter->hurt((*current)->getDamage());
					if((*current)->getcarryBuff()!=nullptr)
						_myFighter->takeBuff((*current)->getcarryBuff());
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
			if (!enemySoldier.empty())
			{
				for (auto& it : enemySoldier)
				{
					if (it->getPosition().getDistance((*current)->getPosition()) < (*current)->getOwnerWeapon()->getAttackRadius())
					{
						it->takeDamage(DamageMode::COMMON, (*current)->getDamage(), (*current)->getOwner());
					}
				}
			}
			if (!enemyBoss.empty())
			{
				for (auto& it : enemyBoss)
				{
					if (it->getPosition().getDistance((*current)->getPosition()) < (*current)->getOwnerWeapon()->getAttackRadius())
					{
						it->takeDamage(DamageMode::COMMON, (*current)->getDamage(), (*current)->getOwner());
					}
				}
			}
			if ((*current)->getTag() != 725)
			{
				if (nowTime - (*current)->getGiveOutTime() > 1.0 / 15.0f)
					(*current)->setIsToClean(true);
			}
			else
			{
				if (nowTime - (*current)->getGiveOutTime() > 2.0 / 5.0f)
					(*current)->setIsToClean(true);
			}

		}
		else
		{
			auto it = dynamic_cast<Enemy*>((*current)->getOwner());
			if (it->getLevel() == BOSS)
			{
				if (nowTime - (*current)->getGiveOutTime() > 1.0f)
				{
					if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 25 && _myFighter->getCanBeHurt())
					{
						_myFighter->hurt((*current)->getDamage());
						
					}
				}

				if (nowTime - (*current)->getGiveOutTime() > 2.0f)
					(*current)->setIsToClean(true);
			}
			else
			{
				if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 25 && _myFighter->getCanBeHurt())
				{
					_myFighter->hurt((*current)->getDamage());
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

	for (auto i = allComp.begin(); i != allComp.end();)
	{
		if ((*i)->getOwner() == nullptr || (*i)->getOwner()->getAlreadyDead())
		{
			(*i)->setVisible(false);
			this->getMap()->removeChild(*i);
			i = allComp.erase(i);
		}
		else
			++i;
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

void GameScene::updateMusic()
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if (roomState == ROOM_2_3)
	{
		audio->playBackgroundMusic("ArtDesigning/Audio/Boss.mp3",true);
	}
	else
	{
		audio->playBackgroundMusic("ArtDesigning/Audio/Normal.mp3", true);
	}
}

//帧更新??我看不出来在哪里调用了该函数，但是确实调用了
void GameScene::update(float delta)
{
	updateFighterPosition();
	updateFlyingItem();
	updateComp();
	updateUI();
	updateItem();
	updateSpecialBullet();
	updateEnemyPosition();
	updateNPC();
	clearObject();
}

bool GameScene::islegal(int X,int Y,Stage_State _stage)
{
	if (_stage==ROOM_1_1)
	{
		if (!(X>= 32 * 46 && X <= 64 * 32 && Y >= 17 * 32 && Y <= 32 * 32))
			return false;
		return true;
	}

	if (_stage == ROOM_2_1)
	{
		if (!(X >= 35 * 32 && X <= 51 * 32 && Y >= 15 * 32 && Y <= 32 * 28))
			return false;
		return true;
	}

	if (_stage == ROOM_2_3)
	{
		if (!(X >= 40 * 32 && X <= 59 * 32 && Y >= 40 * 32 && Y <= 32 * 58))
			return false;
		return true;
	}

	return false;
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

void GameScene::toNextScene(float delta)
{
	auto nextScene = Stage_2::create(_ptesmanSelect);
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
}

void GameScene::endGame()
{
	unscheduleUpdate();
	_eventDispatcher->removeAllEventListeners();
	if (_myFighter->getAlreadyDead())
	{
		auto nextScene = DieScene::create();
		Director::getInstance()->replaceScene(
			TransitionSlideInT::create(1.0f / 60, nextScene));
	}
	else
	{
		auto nextScene = SafetyMap::create();
		Director::getInstance()->replaceScene(
			TransitionSlideInT::create(1.0f / 60, nextScene));
	}
}