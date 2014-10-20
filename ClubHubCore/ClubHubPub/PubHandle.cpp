#include "PubHandle.h"

#include "ContentManager.h"
#include "Graphics.h"
#include "Font.h"
#include "Character.h"

#include "UserController.h"
#include "CoordinateTransform.h"

#include "glm\glm.hpp"

#include "Game.h"
Texture *cow;
Font *font;
Game * game;


void PubHandle::init( ContentManager* content, CoordinateTransform *transform )
{
	transform->setCoordinateSystem( CoordinateSystem::SCREEN_COORDINATES );

	cow = content->loadTexture( "Resources/cow.png" );
	font = content->loadFont( "Resources/Tahoma.bfnt" );

	game = new Game();
	game->subscribeContentManager( content );
}

void PubHandle::paint( Graphics* graphics )
{
	game->subscribeGraphicsHandle( graphics );
	game->draw();
}

void PubHandle::update( float dt, const UserController *uc )
{
	game->subscribeUserController( uc );
	game->update( dt );
	dt;
	uc;
}