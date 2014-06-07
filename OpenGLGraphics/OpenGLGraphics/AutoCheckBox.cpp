#include "AutoCheckBox.h"

#include <QtGui\qhboxlayout>
#include <QtGui\qcheckbox.h>

AutoCheckBox::AutoCheckBox( const char * name, bool * data )
{
	QLayout* layout = new QHBoxLayout();

	check = new QCheckBox( name );

	setLayout( layout );
	layout->addWidget(check);

	this -> data = data;

	check->setChecked( *data );

	connect(check, SIGNAL(toggled(bool)), 
		this, SLOT(checkValueChanged()));
	checkValueChanged();
}

bool AutoCheckBox::isChecked() const
{
	return check->isChecked();
}

void AutoCheckBox::setChecked(bool newValue)
{
	check->setChecked( newValue );
	checkValueChanged();
}

void AutoCheckBox::checkValueChanged()
{
	*data = isChecked();
	emit checkChanged(isChecked());
}