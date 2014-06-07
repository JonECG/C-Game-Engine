#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"
#include "ForceGenerator.h"


struct VectorGraphic;

class Lab07Gui : public GuiBase
{
	int numParticles;

	VectorGraphic ** particleGraphics;
	Particle * particles;

	VectorGraphic * velocityOneGraphic;
	VectorGraphic * velocityTwoGraphic;
	
	
	ParticleForceGenerator *forceGravity, *forceKeyboard;
	ParticleDrag *forceDrag;
	ParticleSpring **forceSprings;
	ParticleForceRegistry registry;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab07Gui();
	~Lab07Gui();

	void reset();
	void resolveCollision();

	void initialize();
};

