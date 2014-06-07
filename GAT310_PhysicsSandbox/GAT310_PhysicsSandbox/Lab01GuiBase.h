#pragma once
#include "PhysicsGui\GuiBase.h"
#include "glm/glm.hpp"


struct VectorGraphic;

class Lab01GuiBase : public GuiBase
{
	VectorGraphic * centralGraphic;
	VectorGraphic * velocityGraphic;
	glm::vec3 position, velocity;

	void newFrame();
	void vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta );

	glm::vec3 vectorFromKeyInput();

public:
	Lab01GuiBase();
	~Lab01GuiBase();

	void initialize();
};

