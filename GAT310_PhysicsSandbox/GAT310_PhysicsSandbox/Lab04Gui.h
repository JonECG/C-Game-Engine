#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"
#include "ForceGenerator.h"


struct VectorGraphic;

class Lab04Gui : public GuiBase
{
	VectorGraphic * particleGraphic;

	VectorGraphic * velocityGraphic;
	VectorGraphic * momentumGraphic;
	
	Particle particle;
	ParticleForceGenerator *forceGravity, *forceKeyboard;
	ParticleDrag *forceDrag;
	ParticleForceRegistry registry;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab04Gui();
	~Lab04Gui();

	void initialize();
};

