#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"
#include "ForceGenerator.h"
#include "ParticleWorld.h"

struct VectorGraphic;

class Lab15Gui : public GuiBase
{
	int numParticles;
	int MAXPARTICLE;

	VectorGraphic ** particleGraphics;
	ParticleWorld world;
	
	ParticleSpring *mainSpring;
	ParticleForceGenerator *forceGravity, *forceKeyboard;
	ParticleDrag *forceDrag;
	ParticleSpring **forceSprings;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	void makeConstraints();

	glm::vec3 vectorFromKeyInput();

	int springCount;

public:
	Lab15Gui();
	~Lab15Gui();

	void reset();
	void throwing();
	void catching();
	void resolveCollision( float dt );

	void initialize();
};

