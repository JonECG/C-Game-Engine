#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Window\Graphics.h"
#include "Renderer\Renderable.h"
#include "Entity.h"
#include "Game.h"
#include "Stage.h"

RenderComponent::RenderComponent()
{
}

void RenderComponent::setRenderable( Renderable * renderable )
{
	this->renderable = renderable;
}

void RenderComponent::draw()
{
	Graphics * g = parent->getStage()->getGame()->getGraphicsHandle();
	TransformComponent * trans = parent->getComponent<TransformComponent>();
	if( trans != nullptr )
	{
		g->setTransform( trans->getTransformation() );
	}
	else
	{
		g->setTransform( glm::mat4() );
	}
	g->drawRenderable( renderable );
}
