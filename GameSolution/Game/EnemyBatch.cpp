#include "EnemyBatch.h"
#include "ChaseEnemy.h"
#include "TankEnemy.h"
#include "MathHelper.h"

EnemyBatch::EnemyBatch()
{
	head = 0;
	this->size = ENEM_LIMIT;
	current = new Enemy*[size];
	for( int i = 0; i < size; i++ )
	{
		current[i] = new ChaseEnemy();
	}
}

Enemy * EnemyBatch::createEnemy()
{
	int i = 0;
	while(current[(head+i)%ENEMY_LIMIT]->isAlive()&&i < ENEMY_LIMIT)
	{
		i++;
	}

	Enemy * renew = static_cast<Enemy*>(current[(head+i)%ENEMY_LIMIT]);

	head = (head + i + 1)%ENEMY_LIMIT;

	delete renew;
	if (MathHelper::randomFloat() > 0.5f)
	{
		renew = new ChaseEnemy();
	}
	else
	{
		renew = new TankEnemy();
	}

	renew->alive = true;

	return renew;
}