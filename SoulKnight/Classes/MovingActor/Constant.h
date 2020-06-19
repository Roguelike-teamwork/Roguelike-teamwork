#pragma once
#ifndef __CONSTANT_H_
#define __CONSTANT_H_
#include"cocos2d.h"

#define INIT_HITPOINTS 5			//��ʼ������ֵ
#define INIT_SHIELD 5				//��ʼ������ֵ
#define INIT_MANAPOINTS 180			//��ʼ������ֵ
#define COMMON_ATTACKSPEED 1.0f
#define SHIELD_RECOVER_TIME 1.0f     //���ǻ��ܻ��ƺ�ˢ��ʱ���ʼ��1.0f
#define INIT_ID_RADIUS 300        //�ƶ���ɫ�ĸ�Ӧ�뾶��֪̽��Χ���Ƿ��������ƶ�����  

#define	KNIGHT_HP 100
#define KNIGHT_MP 400
#define KNIGHT_SHIELD 50
#define KNIGHT_MOVESPEED 4
#define KNIGHT_IDR 250
#define KNIGHT_SKILLTIME 5.0f
#define KNIGHT_SKILLCD 10.0f

#define GOBLIN_HP 50
#define GOBLIN_DAMAGE 5
#define GOBLIN_FLYSPEED 10
#define GOBLIN_MOVESPEED 30
#define GOBLIN_IDR 350
#define GOBLIN_ATTACKR 275
#define GOBLIN_ATTACKSPEED 1

#define PIG_HP 200
#define PIG_DAMAGE 30
#define PIG_MOVESPEED 80
#define PIG_IDR 400
#define PIG_ATTACKR 50
#define PIG_ATTACKSPEED 2

#define DRAGON_HP 1500
#define DRAGON_DAMAGE 4
#define DRAGON_MOVESPEED 60
#define DRAGON_FLYSPEED 7

#define PISTOL_DAMAGE 7
#define PISTOL_ATTACKSPEED 0.7
#define PISTOL_FLYSPEED 16
#define PISTOL_MPCONSUME 5

#define FORK_DAMAGE 4
#define FORK_ATTACKSPEED 1.0
#define FORK_MPCONSUME 2



#define INIT_MOVESPEED 3.0
#define INIT_EQUIP_NUMBER 2

#define TAG_layer_Score 0 


enum EAttackMode	//	����ģʽ,�����������Ĺ���ģʽ��Ҳ���ǹ���Ĺ���ģʽ
{
	MELEE,			//	��ս
	REMOTE,			//	Զ��
	MIX				//  ���
};

enum WeaponStatus
{
	GROUND,
	TAKEN
};

enum EnemyLevel
{
	SOLDIER,
	BOSS
};

enum DamageMode      //�˺�����	
{
	POISONING,
	COMMON,
};

enum AllCamp        //��Ӫ
{
	FRIENDLY,
	ENEMY
};
//���ܻᶨ���
enum EEQUIPMENT
{
	POJIUDESHOUQIANG,
	AK47,
	GANCAOCHA,
	HAND
	//TBD
};

//���ܻᶨ��ķ���ö������
enum EDirection
{
	NODIR,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT
};

enum keyPress
{
	PRESS,
	RELEASE
};


//���ܻ�
typedef std::vector<std::vector<bool>> DyaDicVector;
typedef struct PointINT
{
	INT32 x, y;

	PointINT(INT32 x = 0, INT32 y = 0) :x(x), y(y) {}

	PointINT& operator = (const cocos2d::Vec2& point)
	{
		x = static_cast<INT32>(point.x);
		y = static_cast<INT32>(point.y);
		return *this;
	}


}SizeINT;





#endif //!__CONSTANT_H_