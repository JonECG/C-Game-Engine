#pragma once
#include "Component.h"
struct Texture;

class PlanetComponent : public Component
{
	Texture * noiseMap;
	Entity * sunEntity;

	float orbitRate;
	float currentRot;
	float twinkleTime;
public:
	PlanetComponent();
	void init();
	void update( float dt );
};

