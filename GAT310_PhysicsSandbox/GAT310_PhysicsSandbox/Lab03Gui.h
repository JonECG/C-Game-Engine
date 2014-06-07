#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"
#include "ForceGenerator.h"


struct VectorGraphic;

class Lab03Gui : public GuiBase
{
	VectorGraphic * particleGravityGraphic, *particleBothGraphic, *particleKeyboardGraphic;
	VectorGraphic * velocityGraphic;
	VectorGraphic * momentumGraphic;
	
	Particle particleGravity, particleBoth, particleKeyboard;
	ParticleForceGenerator *forceGravity, *forceKeyboard;
	ParticleForceRegistry registry;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab03Gui();
	~Lab03Gui();

	void initialize();
};

