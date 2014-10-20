#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Entity.h"

RenderComponent::RenderComponent()
{
}

void RenderComponent::setRenderable( GeneralGlWindow::Renderable * renderable )
{
	this->renderable = renderable;
}

void RenderComponent::draw()
{
	TransformComponent * trans = parent->getComponent<TransformComponent>();
	if( trans != nullptr )
		renderable->where = trans->getTransformation();
	renderable->draw();
}
