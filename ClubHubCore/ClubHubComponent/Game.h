#pragma once
#include "definition.h"
class Stage;
class Graphics;
class UserController;
class ContentManager;

class EXPORT Game
{
	Graphics * graphics;
	ContentManager * content;
	const UserController * uc;
	Stage * stage;
public:
	Game();
	~Game();
	void update( float dt );
	void draw();
	void setStage( Stage * stage );
	Stage * getStage();

	void subscribeGraphicsHandle( Graphics * g );
	Graphics * getGraphicsHandle();
	void subscribeUserController( const UserController * uc );
	const UserController * getUserController();
	void subscribeContentManager( ContentManager * content );
	ContentManager * getContentManager();
};

