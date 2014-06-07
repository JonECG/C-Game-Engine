#ifndef ENEMYBATCH_H
#define ENEMYBATCH_H

#include "Enemy.h"
#include "DebugMemory.h"
#include "OrganicContainer.h"

//Stores and maintains enemy pool
class EnemyBatch : public OrganicContainer<Enemy>
{
	static const int ENEM_LIMIT = 100;
public:
	static const int ENEMY_LIMIT = ENEM_LIMIT;
	EnemyBatch();
	Enemy * createEnemy();
};

#endif