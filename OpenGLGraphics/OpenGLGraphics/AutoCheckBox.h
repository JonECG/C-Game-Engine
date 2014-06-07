#include <QtGui\qwidget>
class QCheckBox;

class AutoCheckBox : public QWidget
{
	Q_OBJECT

	QCheckBox* check;
	bool * data;

	private slots:
		void checkValueChanged();

	signals:
		void checkChanged(bool newValue);

	public:
		AutoCheckBox( const char * name, bool * data );
		bool isChecked() const;
		void setChecked(bool newValue);
};