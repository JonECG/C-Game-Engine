#pragma once
#include <QtGui\qwidget>
#include <QtGui\qtabwidget.h>
#include "AutoCheckBox.h"
#include "DebugSlider.h"
#include "VectorEditWidget.h"
#include "AutoLabel.h"
#include "Engine.h"
#include <window.h>

class ENGINE_SHARED DebugMenus : public QWidget
{
private:
	QTabWidget* widg;
	int currentTab;
	QWidget* tabs[10];
	const char* names[10];

	inline QWidget* fetchTab( const char* tabName )
	{
		QWidget* result = nullptr;
		for( int i = 0; i < currentTab; i++ )
		{
			if ( std::strcmp( names[i], tabName ) == 0 )
			{
				result = tabs[i];
			}
		}

		if( result == nullptr )
		{
			result = new QWidget();
			widg->addTab( result, tabName );
			tabs[currentTab] = result;
			names[currentTab] = tabName;
			result->setLayout( new QVBoxLayout() );
			currentTab++;
		}
		return result;
	}
public:
	static DebugMenus *menu;

	

#ifdef DEBUG_MENUS

	inline DebugMenus()
	{
		currentTab = 0;
		QVBoxLayout *mainLayout = new QVBoxLayout();
		widg = new QTabWidget();
		resize( 500, 500 );

		setLayout(mainLayout);
		layout()->addWidget( widg );
	}

	inline static void watchFloat( char* description, float& data, const char* tabName = "<DEFAULT>" )
	{
		float* dataPointer = &data;
		menu->fetchTab(tabName)->layout()->addWidget( new AutoLabel( description, dataPointer ) );
	}

	inline static void watchText( char* description, const char * const & data, const char* tabName = "<DEFAULT>" )
	{
		const char * const * dataPointer = &data;
		menu->fetchTab(tabName)->layout()->addWidget( new AutoLabel( description, dataPointer ) );
	}

	inline static void slideFloat( char* description, float& data, float min = 0.0f, float max = 1.0f, const char* tabName = "<DEFAULT>" )
	{
		float* dataPointer = &data;
		menu->fetchTab(tabName)->layout()->addWidget( new VectorEditWidget( description, dataPointer, min, max ) );
	}

	template <class Vect>
	inline static void slideVector( char* description, Vect& data, float min = 0.0f, float max = 1.0f, const char* tabName = "<DEFAULT>" )
	{
		Vect* dataPointer = &data;
		menu->fetchTab(tabName)->layout()->addWidget( new VectorEditWidget( description, dataPointer, min, max ) );
	}

	inline static void toggleBool( char* description, bool& data, const char* tabName = "<DEFAULT>" )
	{
		bool* dataPointer = &data;
		menu->fetchTab(tabName)->layout()->addWidget(new AutoCheckBox( description, dataPointer ) );
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

	inline static void watchFloat( char* description, float& data, const char* tabName = "<DEFAULT>" ){}

	inline static void watchText( char* description, const char * const & data, const char* tabName = "<DEFAULT>" ){}

	inline static void slideFloat( char* description, float& data, float min = 0.0f, float max = 1.0f, const char* tabName = "<DEFAULT>" ){}

	template <class Vect>
	inline static void slideVector( char* description, Vect& data, float min = 0.0f, float max = 1.0f, const char* tabName = "<DEFAULT>" ){}

	inline static void toggleBool( char* description, bool& data, const char* tabName = "<DEFAULT>" ){}

	inline static void update(){}

	inline static void inject( QLayout* layout ){}

#endif
};