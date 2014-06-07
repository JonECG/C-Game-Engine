#ifndef BULLETBATCH_H
#define BULLETBATCH_H

#include "Enemy.h"
#include "DebugMemory.h"
#include "OrganicContainer.h"

//Stores and maintains enemy pool
class BulletBatch : public OrganicContainer<Bullet>
{
public:
	static const int BULL_LIMIT = 100;
	BulletBatch();
	Bullet * createBullet();
};

#endif