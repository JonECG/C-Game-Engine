#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"
#include "ForceGenerator.h"


struct VectorGraphic;

class Lab06Gui : public GuiBase
{
	int numParticles;

	VectorGraphic ** particleGraphics;
	Particle * particles;

	VectorGraphic * velocityGraphic;
	VectorGraphic * momentumGraphic;
	
	
	ParticleForceGenerator *forceGravity, *forceKeyboard;
	ParticleDrag *forceDrag;
	ParticleSpring **forceSprings;
	ParticleForceRegistry registry;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab06Gui();
	~Lab06Gui();

	void initialize();
};

