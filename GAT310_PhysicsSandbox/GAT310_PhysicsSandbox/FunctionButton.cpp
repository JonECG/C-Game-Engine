#include "FunctionButton.h"


FunctionButton::FunctionButton( const char * name, std::function<void()> func ) : QPushButton( name )
{
	this->func = func;

	connect(this, SIGNAL(clicked()), 
		this, SLOT(runFunction()));
}

void FunctionButton::runFunction()
{
	func();
}