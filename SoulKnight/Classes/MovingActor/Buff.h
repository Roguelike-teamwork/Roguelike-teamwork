#pragma once
#ifndef __BUFF_H__
#define __BUFF_H__
#include "cocos2d.h"

USING_NS_CC;

enum EBuffType
{
	VERTIGO,	//ѣ��
	FROZEN,     //����
	BURN,       //����
	POISON,     //�ж�
	SPEEDUP,    //����
	SPEEDDOWN,  //����
	NORMAL		//����,Ҳ����Ѫƿ��ħƿ
};
//��ҩƿ�ɹ���buff����
class Buff :public cocos2d::Sprite
{
	//CC_SYNTHESIZE(int, hp, HP);                          
	//CC_SYNTHESIZE(float, buffMoveSpeed, BuffMoveSpeed);

	CC_SYNTHESIZE(EBuffType,buffType,BuffType);			//buff������
	CC_SYNTHESIZE(int, buffHp, BuffHp);                   //ѪƿЧ��,���ˣ��ж�
	CC_SYNTHESIZE(int, buffMp, BuffMp);					 //ħƿЧ��       ����ҩƿЧ��
	CC_SYNTHESIZE(float, buffMoveSpeed, BuffMoveSpeed);   //�ƶ��ٶ�Ч��   ����/���� ,������ѣ��
	

	CC_SYNTHESIZE(float, duration, Duration);   //����ʱ��
	CC_SYNTHESIZE(float, beginTime, BeginTime); //��ʼʱ��
	CC_SYNTHESIZE(float, endTime, EndTime);		//����ʱ��
public:
	bool init(EBuffType _bufftype,
		int _buffHP, int _buffMP,
		float _buffMoveSpeed, float _duration);
	
	static Buff* create(EBuffType _bufftype,
		int _buffHP, int _buffMP,
		float _buffMoveSpeed, float _duration);
};




#endif // !__BUFF_H__
