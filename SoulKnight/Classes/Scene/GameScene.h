#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"
#include "MovingActor/Fighter.h"
#include "MovingActor/MovingActor.h"
#include "MovingActor/Bullet.h"
#include "MovingActor/UnMovingActor.h"
#include "Comp/Comp.h"
#include "Controller/MoveCtrl.h"
#include <vector>

USING_NS_CC;

class Record;
class MovingActor;

class GameScene : public cocos2d::Scene
{
	/*
	CC_SYNTHESIZE(varType, varName, funName)这个宏是用来
	定义变量varName，其类型为varType，并生成其get/set方法，分别为：
		getfunName() 和 setfunName(varType var) 。
	*/

	//CC_SYNTHESIZE(Vector<Projectile*>, _bullets, Bullets);      子弹
	//CC_SYNTHESIZE(std::vector<Damage>, _damages, AllDamages);   伤害
	CC_SYNTHESIZE(Record*, _labelRecord, LabelRecord);
	CC_SYNTHESIZE(Fighter*, _myFighter, MyFighter);
	CC_SYNTHESIZE(MoveController*, _rocker, Rocker);
	CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
	CC_SYNTHESIZE(UnMovingActor*,currentNPC,CurrentNPC);
	CC_SYNTHESIZE(Vec2, _origin, Origin);
	CC_SYNTHESIZE(TMXTiledMap*, _map, Map);
	CC_SYNTHESIZE(Equipment*,tempWeapon,TempWeapon);
	CC_SYNTHESIZE(int, coinNum, CoinNum);
	//CC_SYNTHESIZE(MapInfo, _mapInformation, MapInformation);
	cocos2d::TMXLayer* _collidable;
	cocos2d::TMXLayer* _collidable2;
	cocos2d::TMXLayer* _fenceBool;
	cocos2d::TMXLayer* _collisionForFight;
	LabelTTF* hitpoints;
	LabelTTF* shield;
	LabelTTF* manapoints;
	Label* coin;
	Sprite* coinPicture;
	bool collisionForFightBool = false;
private:
	//初始化
	void generateEnemies(float delta);
	void generateComp();
	void initMapLayer();
	void initFighter();
	void initListener();
	void initComp();
	void initCoin();
	void initNextScene();
	void initSettingMenu();
	//update函数合集
	void updateFighterPosition();
	void updateNPC();
	void updateEnemyPosition();
	void updateFlyingItem();
	void updateSpecialBullet();
	void updateComp();
	void updateWeapon();
	void clearObject();
	bool _gameBegin;

	//更新攻击目标   这个好像没用
	void updateEnemiesAttackTarget();

	//监听器
	EventListenerTouchOneByOne* listenerTouch;
	EventListenerKeyboard* listenerKeyBoard;
	//
	void menuPlayCallBack(cocos2d::Ref* pSender);
	void menuMenuCallBack(cocos2d::Ref* pSender);

	//按键事件
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event* event);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event* event);

	//判断是否一直被按住
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	
	void setViewpointCenter(Vec2);
	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);

	MenuItemImage* menuMenu;
public:
	//可以公开使用的一些容器
	Vector<Enemy*> enemySoldier;
	Vector<Enemy*> enemyBoss;
	Vector<Enemy*> allEnemy;
	Vector<MovingActor*> allFighter;
	Vector<Equipment*>allWeapon;
	Vector<Bullet*> flyingItem;
	Vector<Bullet*> specialBullet;
	Vector<UnMovingActor*> allNpc;
	Vector<Comp*> allComp;


	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void loadingAnimation();
	virtual void update(float delta);
	void CircleDamage(Point point, float radius, float damage);
	
	CREATE_FUNC(GameScene);
};



#endif 