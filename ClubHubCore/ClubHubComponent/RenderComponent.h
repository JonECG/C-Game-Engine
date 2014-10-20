#pragma once
#include "Component.h"
#include "definition.h"
struct Renderable;

class EXPORT RenderComponent : public Component
{
	Renderable * renderable;

public:
	RenderComponent();
	void setRenderable( Renderable * renderable );
	void draw();
};

