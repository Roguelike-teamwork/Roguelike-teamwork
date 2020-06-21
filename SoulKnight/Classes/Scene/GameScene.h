#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"
#include "MovingActor/Fighter.h"
#include "MovingActor/MovingActor.h"
#include "MovingActor/Bullet.h"
#include "MovingActor/UnMovingActor.h"
#include "Scene/testmanSelectScene.h"
#include "Comp/Comp.h"
#include "Controller/MoveCtrl.h"
#include <vector>

USING_NS_CC;

class Record;
class MovingActor;

class GameScene : public cocos2d::Scene
{
	/*
	CC_SYNTHESIZE(varType, varName, funName)�����������
	�������varName��������ΪvarType����������get/set�������ֱ�Ϊ��
		getfunName() �� setfunName(varType var) ��
	*/

	//CC_SYNTHESIZE(Vector<Projectile*>, _bullets, Bullets);      �ӵ�
	//CC_SYNTHESIZE(std::vector<Damage>, _damages, AllDamages);   �˺�
	CC_SYNTHESIZE(testmanSelect*, _ptesmanSelect, PtestmanSelect);
	CC_SYNTHESIZE(Stage_State, roomState, RoomState);
	CC_SYNTHESIZE(Record*, _labelRecord, LabelRecord);
	CC_SYNTHESIZE(Fighter*, _myFighter, MyFighter);
	CC_SYNTHESIZE(MoveController*, _rocker, Rocker);
	CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
	CC_SYNTHESIZE(UnMovingActor*,currentNPC,CurrentNPC);
	CC_SYNTHESIZE(Vec2, _origin, Origin);
	CC_SYNTHESIZE(TMXTiledMap*, _map, Map);
	CC_SYNTHESIZE(Equipment*,tempWeapon,TempWeapon);
	CC_SYNTHESIZE(int, coinNum, CoinNum);
	CC_SYNTHESIZE(bool, poisonState, PoisonState);
	CC_SYNTHESIZE(float, poisonTime, PoisonTime);
	//CC_SYNTHESIZE(MapInfo, _mapInformation, MapInformation);
	cocos2d::TMXLayer* _collidable;
	cocos2d::TMXLayer* _collidable2;
	cocos2d::TMXLayer* _fence;
	cocos2d::TMXLayer* _fenceBool;
	cocos2d::TMXLayer* _collisionForFight;
	cocos2d::TMXLayer* _transfer;
	cocos2d::TMXLayer* _enemyCollision;
	cocos2d::TMXLayer* _spikeweed;
	Label* coin;
	Sprite* coinPicture;
	bool collisionForFightBool = false;
public:
	//��ʼ��
	bool islegal(int X,int Y,Stage_State _stage);
	void toNextScene(float delta);
	void generateComp();
	void initFighter();
	void initListener();
	void initComp();
	void initController();
	void initCoin();
	void initUI();
	void initSettingMenu();
	void updateFighterPosition();
	void updateNPC();
	void updateUI();
	void updateEnemyPosition();
	void updateFlyingItem();
	void updateSpecialBullet();
	void updateComp();
	void updateWeapon();
	//����
	void updateGrade();
	void clearObject();
	bool _gameBegin;

	//���¹���Ŀ��   �������û��
	void updateEnemiesAttackTarget();

	//������
	EventListenerTouchOneByOne* listenerTouch;
	EventListenerKeyboard* listenerKeyBoard;
	//
	void menuMenuCallBack(cocos2d::Ref* pSender);

	//�����¼�
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event* event);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event* event);

	//�ж��Ƿ�һֱ����ס
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	
	void setViewpointCenter(Vec2);
	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);

	MenuItemImage* menuMenu;

	//���Թ���ʹ�õ�һЩ����
	Vector<Enemy*> enemySoldier;
	Vector<Enemy*> enemyBoss;
	Vector<Enemy*> allEnemy;
	Vector<MovingActor*> allFighter;
	Vector<Equipment*>allWeapon;
	Vector<Bullet*> flyingItem;
	Vector<Bullet*> specialBullet;
	Vector<UnMovingActor*> allNpc;
	Vector<Buff*> allBuff;
	Vector<Sprite*> allUI;
	Vector<Comp*> allComp;
	Vector<Sprite*> item;

	static GameScene* create(testmanSelect* scene);
	static cocos2d::Scene* createScene(testmanSelect* scene);

	virtual void updateMusic();
	virtual void updateItem();
	virtual bool init(testmanSelect* scene);
	virtual void loadingAnimation();
	virtual void update(float delta);
	void CircleDamage(Point point, float radius, float damage);
	void endGame();

};



#endif 