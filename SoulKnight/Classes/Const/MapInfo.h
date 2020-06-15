#pragma once
#ifndef __MAPINFO_H__
#define __MAPINFO_H__

#include "cocos2d.h"
#include "MovingActor/Constant.h"

USING_NS_CC;
class MapInfo
{
private:

	CC_SYNTHESIZE(SizeINT, _mapSize, MapSize);

	SizeINT _tileSize;

	DyaDicVector _collisionBoard;

public:

	MapInfo() = default;

	MapInfo(TMXTiledMap* map);

	PointINT getGridPoint(const Vec2& position) const;

	//void removeTowerCollision(const Vec2& towerPosition);//可以换成可以被击毁的箱子

	Vec2 getPrecisePosition(const PointINT& girdPoint) const;

	bool checkCollision(const Vec2& position) const;
};

#endif // !__MAPINFO_H__