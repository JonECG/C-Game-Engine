#ifndef GUI_BASE_H
#define GUI_BASE_H
#include <QtGui\qwidget.h>
#include <Qt\qtimer.h>
#include <glm\glm.hpp>
#include <ExportImport.h>
#include <PhysicsGui\VectorGraphic.h>
class QVBoxLayout;
class CartesianGraph;
class QCheckBox;
class QLabel;
class ThreeDGraph;

class ME_PHYSICS_DLL_SHARED GuiBase : public QWidget
{		
	Q_OBJECT
	QTimer timer;

	float targetDt;
	QVBoxLayout* mainLayout;
	QCheckBox* autoStepCbx;
	QVBoxLayout* controlsLayout;
	CartesianGraph* graph;
	ThreeDGraph* threeDGraph;
	QLabel* fpsLabel;

	unsigned int twoDGridSize;
	bool is3D;

	private slots:
		void initializeGraphics();
	void timerTicked();
protected slots:
	void nf(); // newFrame
protected:
	QVBoxLayout* debugLayout;
	virtual void newFrame();
	void setFps(int targetFps);
	float dt() const;
	bool isKeyDown(const char* key) const;
protected slots:
	virtual void vectorGraphicMouseDrag(uint vectorGraphicIndex, const glm::vec3& dragDelta);
public:
	GuiBase();
	virtual void initialize(bool threeD = true, bool autoStep = false);
	VectorGraphic* addVectorGraphic();
	static void sync(VectorGraphic* graphic, const glm::vec3& position, const glm::vec3& base = glm::vec3());
	glm::vec3 getMousePosition() const;
};


#endif