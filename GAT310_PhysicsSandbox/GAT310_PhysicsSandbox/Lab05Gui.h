#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"
#include "ForceGenerator.h"


struct VectorGraphic;

class Lab05Gui : public GuiBase
{
	int numParticles;

	VectorGraphic ** particleGraphics;
	Particle * particles;

	VectorGraphic * velocityGraphic;
	VectorGraphic * momentumGraphic;
	
	
	ParticleForceGenerator *forceGravity, *forceKeyboard;
	ParticleDrag *forceDrag;
	ParticleSpring *forceSpring;
	ParticleForceRegistry registry;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab05Gui();
	~Lab05Gui();

	void initialize();
};

