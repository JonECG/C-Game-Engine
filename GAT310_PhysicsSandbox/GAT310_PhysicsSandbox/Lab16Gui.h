#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"
#include "ForceGenerator.h"
#include "ParticleWorld.h"

struct VectorGraphic;

class Lab16Gui : public GuiBase
{
	int numParticles;

	ParticleWorld world;

	ParticleForceGenerator *forceGravity, *forceKeyboard;
	ParticleDrag *forceDrag;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab16Gui();
	~Lab16Gui();

	void reset();
	void initialize();
};

