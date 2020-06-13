#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"
#include "MovingActor/Knight.h"
#include "MovingActor/MovingActor.h"
#include "MovingActor/Bullet.h"
#include "Controller/MoveCtrl.h"
#include "Const/MapInfo.h"
#include <vector>

USING_NS_CC;

class Record;

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
	CC_SYNTHESIZE(Knight*, _myKnight, MyKnight);
	CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
	CC_SYNTHESIZE(Vec2, _origin, Origin);
	CC_SYNTHESIZE(TMXTiledMap*, _map, Map);
	CC_SYNTHESIZE(MapInfo, _mapInformation, MapInformation);
	cocos2d::TMXTiledMap* _tileMap;
	Fighter* fighter;
private:
	//初始化
	void generateEnemies(float delta);
	void initMapLayer();
	void initFighter();
	void initComp();
	void initListener();
	//更新角色位置
	void updateFighterPosition();
	void setViewpointCenter(CCPoint position);
	void updateFlyingItem();
	void updateComp();
	//更新攻击目标
	void updateEnemiesAttackTarget();

	//监听器
	EventListenerTouchOneByOne* listenerTouch;
	EventListenerKeyboard* listenerKeyBoard;

	//按键事件
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event* event);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event* event);

	//判断是否一直被按住
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	
	void setSpritePoint(CCPoint position);
	CCPoint tileCoordForPosition(CCPoint position);


public:
	//可以公开使用的一些容器
	Vector<MovingActor*> enemySoldier;
	Vector<MovingActor*> enemyBoss;
	Vector<MovingActor*> allFighter;
	Vector<Equipment*> allWeapon;
	Vector<Bullet*> flyingItem;
	//Vector<UnMovingActor*> allNpc;


	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void loadingAnimation();
	virtual void update(float delta);
	void CircleDamage(Point point, float radius, float damage);
	
	CREATE_FUNC(GameScene);
};



#endif 