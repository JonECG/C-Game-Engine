#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"

#include "Particle.h"

struct VectorGraphic;

class Lab02Gui : public GuiBase
{
	VectorGraphic * centralGraphic;
	VectorGraphic * velocityGraphic;
	VectorGraphic * momentumGraphic;
	
	Particle particle;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab02Gui();
	~Lab02Gui();

	void initialize();
};

