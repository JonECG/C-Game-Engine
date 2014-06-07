#include "AutoLabel.h"

#include <QtGui\qhboxlayout>
#include <QtGui\qlabel.h>

AutoLabel::AutoLabel( const char * name, float * data )
{
	QLayout* layout = new QHBoxLayout();

	isFloat = true;

	label = new QLabel( name );

	setLayout( layout );
	layout->addWidget( new QLabel( name ) );
	layout->addWidget(label);

	this -> data = data;

	connect( &timer, SIGNAL(timeout()), this, SLOT(update()) );
	timer.start(0);
}

void AutoLabel::update()
{
	label ->setText( QString::number( *((float*)data) ) );
}