#pragma once
#include "Component.h"
struct Texture;

class PlanetComponent : public Component
{
	Entity * sunEntity;

	float orbitRate;
	float currentRot;
	float twinkleTime;
public:
	static Texture * noiseMap;

	PlanetComponent();
	void init();
	void update( float dt );
};

