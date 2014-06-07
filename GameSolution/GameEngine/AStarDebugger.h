#pragma once
#include "glm/ext.hpp"
#include "GeneralGLWindow.h"
#include "WayMapLevel.h"
#include "Engine.h"
#include "Camera.h"

class ENGINE_SHARED AStarDebugger
{

#ifdef _DEBUG
	GeneralGlWindow * currentWindow;
	GeneralGlWindow::ShaderInfo * currentShader;
	WayMapLevel * currentLevel;

	bool displayPaths, displayCurrentPath, displayNodes, railedCamera;
	Camera *camera;

	WayPath** currentPath;
	float * currentDistance;


public:
	
	AStarDebugger();
	void init( GeneralGlWindow *window, WayMapLevel *level, GeneralGlWindow::ShaderInfo * shader, float &characterSpeed, Camera * camera, WayPath* &currentPath, float &currentDistance );
	void paint();

#else

public:
	inline AStarDebugger(){};
	inline void init( GeneralGlWindow *window, WayMapLevel *level, GeneralGlWindow::ShaderInfo * shader, float &characterSpeed, Camera * camera, WayPath* &currentPath, float &currentDistance ){};
	inline void paint(){};

#endif
};

