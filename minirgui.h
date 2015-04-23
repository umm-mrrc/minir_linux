#ifndef MINIRGUI_H
#define MINIRGUI_H

#include <QMainWindow>

#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <vtkSmartPointer.h>
#include <vtkActor2D.h>
#include <string>
#include "QVTKWidget.h"
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkRenderWindow.h>
#include <vtkDICOMImageReader.h>
#include "endoQuery.h"
#include <QFileDialog>
#include <vtkEventQtSlotConnect.h>

namespace Ui {
class minirGui;
}

struct imgPanel {
  std::string annotation;
  QVTKWidget *p_widget;
  vtkImageViewer2 *p_imageViewer;
  vtkSmartPointer<vtkActor2D> p_dicomAnnotation;
};

class minirGui : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit minirGui(QWidget *parent = 0);
    ~minirGui();
    
private slots:
    void on_endoConnect_clicked();

    void on_test_clicked();

    void on_endoLog_clicked();

    void on_clearStatus_clicked();

public slots:
    void loadImages(vtkObject*);

private:
    Ui::minirGui *ui;
    imgPanel imgPanel_ax;
    imgPanel imgPanel_3D;
    imgPanel imgPanel_cor;
    imgPanel imgPanel_sag;
    endoQuery endoScout;
    vtkEventQtSlotConnect* Connections;

    void loadDicom(std::string, imgPanel *);
    void initImgPanel(imgPanel *);
};

#endif // MINIRGUI_H
