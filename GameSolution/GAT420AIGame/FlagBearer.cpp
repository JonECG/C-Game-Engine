#include "FlagBearer.h"
#include "glm/ext.hpp"

FlagBearer::FlagBearer( GeneralGlWindow::Renderable* renderable, GameWorld * env, Decision * decisionTree, bool isOnRed )
{
	this->renderable = renderable;
	isRed = isOnRed;
	speed = 3;
	this->env = env;

	currentState = nullptr;

	position = env->level->getNode( env->getHomeNode( isRed ) );
	path = nullptr;

	this->decisionTree = decisionTree;

	currentState = nullptr;
	//setState( decisionTree->evaluate( this ) );
}
void FlagBearer::paint()
{
	renderable->draw();
}
void FlagBearer::update(float dt)
{
	if( decisionTree != nullptr )
		setState( decisionTree->evaluate( this ) );
	if ( currentState != nullptr )
		currentState->update( dt, this );
}
void FlagBearer::setState( State * state )
{
	if( state != nullptr && currentState != nullptr && state->getId() == currentState->getId() )
	{
		delete state;
	}
	else
	{
		if( currentState != nullptr )
			delete currentState;

		currentState = state;
		currentState->init( this );
	}
}