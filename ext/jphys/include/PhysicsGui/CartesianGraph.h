#ifndef CARTESIAN_GRAPH_H
#define CARTESIAN_GRAPH_H
#include <QtGui\qwidget.h>
#include <Qt\qlist.h>
#include <glm\glm.hpp>
#include "VectorGraphic.h"
class QPaintEvent;

class CartesianGraph : public QWidget
{
	Q_OBJECT
	QList<VectorGraphic> vectorGraphics;
	const unsigned int GRID_SIZE;
	void drawVector(unsigned int vectorGraphicIndex, QPainter& painter) const;
	void drawCartesianGraph(QPainter& painter);
	void drawVectors(QPainter& painter);
	void checkIfMouseOnVectorGraphic(QMouseEvent*);
	glm::vec2 getPositionInMouseCoords(const VectorGraphic& v) const;
	bool mouseOnThisVectorGraphic(const VectorGraphic& v);

	glm::vec2 minVisible;
	glm::vec2 maxVisible;
	glm::vec2 paintDelta;
	QPoint lastMousePoint;
	QPoint lastAlwaysTrackingMousePoint;
	bool trackingMouse;
	uint vectorGraphicIndexThatMouseIsOn;
	glm::vec2 mouseOffsetOnVectorGraphic;
	glm::vec3 lastReportedVectorGraphicDelta;

signals:
		void vectorGraphicMouseDrag(uint vectorGraphicIndex, const glm::vec3& newPosition);

protected:
	void paintEvent(QPaintEvent*);
	virtual void wheelEvent(QWheelEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
public:
	CartesianGraph(unsigned int gridSize = 20);
	VectorGraphic* addVectorGraphic(const VectorGraphic& vectorGraphic = VectorGraphic());
	glm::vec3 getMousePosition() const;
	void clearVectorGraphics();
	void setVisibleArea(float minX, float maxX, float minY, float maxY);
	bool showGrid;
	bool showCrossHairs;
	bool showXAxis;
	bool showYAxis;
};

#endif