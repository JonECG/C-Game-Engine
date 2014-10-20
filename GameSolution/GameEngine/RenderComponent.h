#pragma once
#include "Component.h"
#include "GeneralGLWindow.h"

class ENGINE_SHARED RenderComponent : public Component
{
	GeneralGlWindow::Renderable * renderable;

public:
	RenderComponent();
	void setRenderable( GeneralGlWindow::Renderable * renderable );
	void draw();
};

