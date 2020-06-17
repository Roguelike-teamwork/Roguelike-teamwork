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
	auto visibleSize = Director::getInstance()->getVisibleSize();//�е����⣺һ��ȥ��ʱ��û�����ú�λ��
	float x = MAX(position.x, visibleSize.width / 2);
	float y = MAX(position.y, visibleSize.height / 2);
	x = MIN(x, (_map->getMapSize().width * _map->getTileSize().width) - visibleSize.width / 2);
	y = MIN(y, (_map->getMapSize().height * _map->getTileSize().height) - visibleSize.height / 2);
	Vec2 pointA = Vec2(visibleSize.width / 2, visibleSize.height / 2);//��Ļ���ĵ�
	Vec2 pointB = Vec2(x, y);//ʹ���鴦����Ļ���ģ��ƶ���ͼĿ��λ�ã�

	//��ͼ�ƶ�ƫ����
	Vec2 offset = pointA - pointB;
	offset.y += 300;	
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
	int x = pos.x / tiledSize.width;//ת��Ϊ�߿�ĸ��������� 
	int y = (mapTiledNum.height* tiledSize.height - pos.y) / tiledSize.height;//ת��Ϊ�߿�ĸ���������
	return ccp(x, y);
}


//���ɵ���
void GameScene::generateEnemies(float delta)
{
	Enemy * enemyMelee_1 = Goblin::create(this, "Goblin");

	//���ض����
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	//��������������
	ValueMap  playerPointDic = objGroup->objectNamed("SpawnPoint");
	float playerX = playerPointDic.at("x").asFloat();
	float playerY = playerPointDic.at("y").asFloat();

	enemyMelee_1->setPosition(ccp(playerX, playerY));
	_map->addChild(enemyMelee_1, 1);
	enemySoldier.pushBack(enemyMelee_1);
}

//��ʼ������,Ŀǰ�ٽ�ɫ�ࣨ�ڶ��ɫ��ѡ��ǰ���£�
void GameScene::initFighter()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	//_myHero->setTag(TAG_MYHERO);
	
	//�������
	//addFighter Դ�� https://blog.csdn.net/u010778159/article/details/43956151?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-2
	//CCSprite* playerSprite = CCSprite::create("ArtDesigning/Sprite/Fighter/KnightDown.png");
	fighter = Knight::create(this, "Knight");
	//fighter->bindSprite(playerSprite);
	allFighter.pushBack(fighter);

	//���ض����
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	//��������������
	ValueMap  playerPointDic = objGroup->objectNamed("SpawnPoint");
	float playerX = playerPointDic.at("x").asFloat();
	float playerY = playerPointDic.at("y").asFloat();

	fighter->setPosition(ccp(playerX, playerY));
	_map->addChild(fighter, 1);
	_myFighter = fighter;
	setViewpointCenter(fighter->getPosition());


	//���ɽ�ɫ��Ӧ������
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
	//�������̼�����
	listenerKeyBoard = EventListenerKeyboard::create();
	//�󶨼����¼�
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
//���ض�����һ������״��
void GameScene::loadingAnimation()
{

}

//��������λ��
void GameScene::updateFighterPosition()
{
	auto nowTime = GetCurrentTime() / 1000.f;


	if (_myFighter->getAlreadyDead())
		return;

	auto it = dynamic_cast<Knight*>(_myFighter);
	it->updateSkill();

	if (it->getCanAttack())
		it->attack();

	_myFighter->setIsMoving(_rocker->getIsCanMove());
	_myFighter->setFDirection(_rocker->getFirstDirection());
	_myFighter->setDirection(_rocker->getDirection());
	_myFighter->setLDriection(_rocker->getLastDirection());
	
	auto newPosition = _myFighter->updateDestination();
	CCPoint tiledpos = tileCoordForPosition(ccp(newPosition.x, newPosition.y));



	//��ײ���
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

	if(_myFighter->getIsMoving())
		_myFighter->fighterMove(newPosition);
	else
		_myFighter->stand();

}

void GameScene::updateEnemyPosition()
{
	auto nowTime = GetCurrentTime() / 1000.f;
	for (auto& i : enemySoldier)
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
			if( !(newPosition.x >= 32 * 18 && newPosition.x <= 31 * 32 && newPosition.y >= 17 * 32 && newPosition.y <= 32 * 32))//ע����Ƭ��ͼ��y��������Ļ�෴
			{
				return;
			}
			CCPoint tiledpos = tileCoordForPosition(ccp(newPosition.x, newPosition.y));
			//��ײ���
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
		//�����ӵ��ķ������������ӵ���������
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
			//��Ϊֻ��һ�����Ǿ���ôд��
			if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 10)
			{
				_myFighter->hurt((*current)->getDamage());
				(*current)->setIsToClean(true);
			}
		}
	}
}

void GameScene::updateSpecialBullet()
{
	auto nowTime = GetCurrentTime()/1000.f;
	for (auto current = specialBullet.begin(); current != specialBullet.end(); current++)
	{
		//��Boss���ĵ������ܲ����ġ��ӵ������д���
		if ((*current)->getOwner()->getCamp() == AllCamp::FRIENDLY)
		{
			//Knight�������һ�ѽ�ս����--����
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
					if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 10)
						_myFighter->hurt((*current)->getDamage());
				}

				if (nowTime - (*current)->getGiveOutTime() > 2.0f)
					(*current)->setIsToClean(true);
			}
			else
			{
				if (_myFighter->getPosition().getDistance((*current)->getPosition()) < 10)
					_myFighter->hurt((*current)->getDamage());
				if(nowTime - (*current)->getGiveOutTime() > 0.3f)
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
}

//֡����??�ҿ�����������������˸ú���������ȷʵ������
void GameScene::update(float delta)
{
	updateFighterPosition();
	updateFlyingItem();
	updateSpecialBullet();
	updateEnemyPosition();
	clearObject();


	/*�ӵ������ж�
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

	/*���������ж�
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

//������Χ
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