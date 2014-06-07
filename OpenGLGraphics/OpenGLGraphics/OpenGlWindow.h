#pragma once
#include "Player.h"
#include "Camera.h"
#include <QtOpenGL\qglwidget>
#include <glm\glm.hpp>
#include <Qt/qtimer.h>

class QMouseEvent;


class OpenGlWindow : public QGLWidget
{
	Q_OBJECT

	QTimer timer;
	
	void mousePressEvent( QMouseEvent * mEvent );
	void setUpShaders();
	void paintPort( Player &player, Camera &camera );
public:
	glm::vec3 objColor;
	glm::vec3 ambLight;
	glm::vec3 diffusePosition;
	glm::vec3 diffuseColor;

	glm::vec3 specColor;
	GLfloat tightness;
	bool useLighting;

	inline OpenGlWindow()
	{
		objColor = glm::vec3( 1, 0, 0 );
		ambLight = glm::vec3( 0.1f, 0.1f, 0.1f );
		diffusePosition = glm::vec3( 0, 5, 0 );
		diffuseColor = glm::vec3( 1, 1, 1 );
		specColor = glm::vec3( 1, 1, 1 );
		tightness = 25;
		useLighting = true;
	}
protected:
	void initializeGL();
	void paintGL();

private slots:
	void update();

};

