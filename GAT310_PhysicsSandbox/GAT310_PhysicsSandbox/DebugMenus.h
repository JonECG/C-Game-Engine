#pragma once
#include <QtGui\qwidget>
#include "AutoCheckBox.h"
#include "DebugSlider.h"
#include "VectorEditWidget.h"
#include "AutoLabel.h"
#include "Engine.h"
#include <windows.h>
#include "FunctionButton.h"

#include "QtGui\qpushbutton.h"
#include <functional>

class ENGINE_SHARED DebugMenus : public QWidget
{
public:

	static DebugMenus *menu;

#ifdef DEBUG_MENUS

	inline DebugMenus()
	{
		QVBoxLayout *mainLayout = new QVBoxLayout();
		setLayout(mainLayout);
	}

	inline static void watchFloat( char* description, float& data )
	{
		float* dataPointer = &data;
		menu->layout()->addWidget( new AutoLabel( description, dataPointer ) );
	}

	inline static void slideFloat( char* description, float& data, float min = 0.0f, float max = 1.0f )
	{
		float* dataPointer = &data;
		menu->layout()->addWidget( new VectorEditWidget( description, dataPointer, min, max ) );
	}

	inline static void actButton( char* title, std::function<void()> func )
	{
		FunctionButton * push = new FunctionButton( title, func );
		menu->layout()->addWidget( push );
	}

	template <class Vect>
	inline static void slideVector( char* description, Vect& data, float min = 0.0f, float max = 1.0f )
	{
		Vect* dataPointer = &data;
		menu->layout()->addWidget( new VectorEditWidget( description, dataPointer, min, max ) );
	}

	inline static void toggleBool( char* description, bool& data )
	{
		bool* dataPointer = &data;
		menu->layout()->addWidget(new AutoCheckBox( description, dataPointer ) );
	}

	inline static void update()
	{
		static short wasTildePressed = 0;
		short isTildePressed = GetAsyncKeyState( 192 );
		if( isTildePressed && !wasTildePressed )
			menu->setVisible( !menu->isVisible() );
		wasTildePressed = isTildePressed;
	}

	inline static void inject( QLayout* layout )
	{
		layout->addWidget( menu );
	}

#else

	inline DebugMenus(){}

	inline static void watchFloat( char* description, float& data ){}

	inline static void slideFloat( char* description, float& data, float min = 0.0f, float max = 1.0f ){}

	template <class Vect>
	inline static void slideVector( char* description, Vect& data, float min = 0.0f, float max = 1.0f ){}

	inline static void toggleBool( char* description, bool& data ){}

	inline static void update(){}

	inline static void inject( QLayout* layout ){}

#endif
};