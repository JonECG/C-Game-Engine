#include "Game.h"
#include "Stage.h"

Game::Game()
{
	graphics = nullptr;
	stage = nullptr;
}
Game::~Game()
{
	if( stage != nullptr )
		delete stage;
}

void Game::update( float dt )
{
	stage->update( dt );
}
void Game::draw()
{
	stage->draw();
}
void Game::setStage( Stage * stage )
{
	if( this->stage != nullptr )
		this->stage->game = nullptr;
	stage->game = this;
	this->stage = stage;
}
Stage * Game::getStage()
{
	return stage;
}


void Game::subscribeGraphicsHandle( Graphics * g )
{
	this->graphics = g;
}
Graphics * Game::getGraphicsHandle()
{
	return graphics;
}
void Game::subscribeUserController( const UserController * uc )
{
	this->uc = uc;
}
const UserController * Game::getUserController()
{
	return uc;
}
void Game::subscribeContentManager( ContentManager * content )
{
	this->content = content;
}
ContentManager * Game::getContentManager()
{
	return content;
}