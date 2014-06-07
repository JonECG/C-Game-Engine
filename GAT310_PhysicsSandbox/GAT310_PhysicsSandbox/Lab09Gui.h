#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"
#include "ForceGenerator.h"


struct VectorGraphic;

class Lab09Gui : public GuiBase
{
	int numParticles;

	VectorGraphic ** particleGraphics;
	Particle * particles;

	/*VectorGraphic * velocityOneGraphic;
	VectorGraphic * velocityTwoGraphic;

	VectorGraphic * totalMomentum;
	VectorGraphic * sepForceOne;
	VectorGraphic * sepForceTwo;*/
	
	
	ParticleForceGenerator *forceGravity, *forceKeyboard;
	ParticleDrag *forceDrag;
	ParticleSpring **forceSprings;
	ParticleForceRegistry registry;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab09Gui();
	~Lab09Gui();

	void reset();
	void rest();
	void resolveCollision( float dt );

	void initialize();
};

