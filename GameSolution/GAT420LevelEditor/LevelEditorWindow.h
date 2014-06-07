#include "Qt\qmainwindow.h"
#include "LevelEditorHandle.h"

#pragma once
class LevelEditorWindow : public QMainWindow
{
	Q_OBJECT

	LevelEditorHandle *handle;

	char * currentPath;
	void saveFile( const char* path );

public:
	LevelEditorWindow();

private slots:
	void loadObj();
	void saveNative();
	void saveNativeAs();
	void loadNative();
};

