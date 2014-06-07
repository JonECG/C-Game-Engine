#include "Ship.h"
#include "MathHelper.h"

Shape* Ship::shipShape = nullptr; //Shape(73,Vector2(30.4f,0.1f),Vector2(13.5f,5.5f),Vector2(6.9f,13.1f),Vector2(-3.1f,19.9f),Vector2(-10.8f,18.5f),Vector2(-8.5f,10.1f),Vector2(-9.6f,7.0f),Vector2(-12.9f,7.3f),Vector2(-17.6f,8.3f),Vector2(-20.9f,11.1f),Vector2(-24.5f,15.0f),Vector2(-24.6f,12.8f),Vector2(-30.7f,14.0f),Vector2(-29.9f,2.7f),Vector2(-24.6f,3.6f),Vector2(-25.1f,0.7f),Vector2(-25.2f,-0.6f),Vector2(-23.1f,-3.4f),Vector2(-9.2f,-3.7f),Vector2(-6.8f,-0.4f),Vector2(-5.1f,-4.8f),Vector2(-4.9f,-9.0f),Vector2(-5.1f,-12.9f),Vector2(-1.0f,-14.0f),Vector2(8.1f,-5.3f),Vector2(4.5f,-4.7f),Vector2(1.9f,-3.2f),Vector2(0.9f,-0.3f),Vector2(8.2f,-5.2f),Vector2(10.8f,-4.7f),Vector2(12.9f,-3.6f),Vector2(14.2f,-2.2f),Vector2(14.9f,-0.4f),Vector2(8.1f,5.1f),Vector2(4.2f,6.7f),Vector2(0.1f,6.5f),Vector2(-3.4f,3.3f),Vector2(-3.4f,-3.3f),Vector2(0.1f,-6.5f),Vector2(4.2f,-6.7f),Vector2(8.1f,-5.1f),Vector2(14.9f,0.4f),Vector2(14.2f,2.2f),Vector2(12.9f,3.6f),Vector2(10.8f,4.7f),Vector2(8.2f,5.2f),Vector2(0.9f,0.3f),Vector2(1.9f,3.2f),Vector2(4.5f,4.7f),Vector2(8.1f,5.3f),Vector2(-1.0f,14.0f),Vector2(-5.1f,12.9f),Vector2(-4.9f,9.0f),Vector2(-5.1f,4.8f),Vector2(-6.8f,0.4f),Vector2(-9.2f,3.7f),Vector2(-23.1f,3.4f),Vector2(-25.2f,0.6f),Vector2(-25.1f,-0.7f),Vector2(-24.6f,-3.6f),Vector2(-29.9f,-2.7f),Vector2(-30.7f,-14.0f),Vector2(-24.6f,-12.8f),Vector2(-24.5f,-15.0f),Vector2(-20.9f,-11.1f),Vector2(-17.6f,-8.3f),Vector2(-12.9f,-7.3f),Vector2(-9.6f,-7.0f),Vector2(-8.5f,-10.1f),Vector2(-10.8f,-18.5f),Vector2(-3.1f,-19.9f),Vector2(6.9f,-13.1f),Vector2(13.5f,-5.5f));
Shape* Ship::turretShape = nullptr; //Shape(15,Vector2(31.5f,0.1f),Vector2(31.5f,-2.1f),Vector2(3.5f,-2.1f),Vector2(0.9f,-7.9f),Vector2(-2.3f,-5.3f),Vector2(-0.7f,-3.1f),Vector2(-1.7f,-1.7f),Vector2(-2.9f,-0.5f),Vector2(-2.9f,0.5f),Vector2(-1.7f,1.7f),Vector2(-0.7f,3.1f),Vector2(-2.3f,5.3f),Vector2(0.9f,7.9f),Vector2(3.5f,2.1f),Vector2(31.5f,2.1f));

Ship::Ship()
{
	position.x = 640;
	position.y = 480;
	rotation = 0;
	turretMatrix = Matrix2();
	reload = FIRE_RATE();
}