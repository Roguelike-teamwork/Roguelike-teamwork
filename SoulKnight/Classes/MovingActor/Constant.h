#pragma once
#ifndef __CONSTANT_H_
#define __CONSTANT_H_
#include"cocos2d.h"

#define INIT_HITPOINTS 5			//初始化生命值
#define INIT_SHIELD 5				//初始化护盾值
#define INIT_MANAPOINTS 180			//初始化法力值
#define COMMON_ATTACKSPEED 1.0f
#define SHIELD_RECOVER_TIME 1.0f     //主角护盾击破后刷新时间初始化1.0f
#define INIT_ID_RADIUS 300        //移动角色的感应半径，探知范围内是否有其他移动物体  

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


enum EAttackMode	//	攻击模式,可以是武器的攻击模式，也可是怪物的攻击模式
{
	MELEE,			//	近战
	REMOTE,			//	远程
	MIX				//  混合
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

enum DamageMode      //伤害类型	
{
	POISONING,
	COMMON,
};

enum AllCamp        //阵营
{
	FRIENDLY,
	ENEMY
};
//可能会定义的
enum EEQUIPMENT
{
	POJIUDESHOUQIANG,
	AK47,
	GANCAOCHA,
	HAND
	//TBD
};

//可能会定义的方向枚举类型
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


//可能会
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