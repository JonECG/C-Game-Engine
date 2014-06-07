#ifndef SECRET_H
#define SECRET_H

#include <vector>
#include "Bullet.h"
#include "Ship.h"
#include "Shape.h"

class Secret
{
	Vector2 position;
	static const int CENTER = 0, LASER = 1, SHOOT = 2, POLITELYWAITING = 3;
	float spin;
	float time;
	float reload;
	int life;
	int state;
public:
	static Shape* jamie;
	Secret();
	void init();
	void wait();
	void hit();
	void disable();
	bool collideWithEye( Vector2 pos, Ship& ship );
	bool collide( Ship& ship );
	void update( float dt, Ship& ship, std::vector<Bullet>& bul );
	void draw( Core::Graphics& g, Matrix4 globalTransform );
	void drawEyeLine( Vector2 position, Core::Graphics& g, Matrix4 globalTransform, int thickness );
	inline int getLife(){ return life; }
	inline Vector2 getPosition(){ return position; }
};

#endif