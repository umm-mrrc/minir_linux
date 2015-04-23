#include "minirgui.h"
#include "ui_minirgui.h"

// Load a DICOM image into the desired panel...
void minirGui::loadDicom(std::string dcmFname, imgPanel *p_imgPanel)
{
  vtkSmartPointer<vtkDICOMImageReader> p_reader =
    vtkSmartPointer<vtkDICOMImageReader>::New();
  p_reader->SetFileName(dcmFname.c_str());
  p_reader->Update();
  p_imgPanel->p_imageViewer->SetInputConnection(p_reader->GetOutputPort());
  p_imgPanel->p_imageViewer->GetRenderWindow()->Render();
  // ???
  p_imgPanel->p_imageViewer->Render();
  p_imgPanel->p_imageViewer->GetRenderer()->ResetCamera();
  p_imgPanel->p_imageViewer->Render();

  float *position = p_reader->GetImagePositionPatient();
  float *orientation = p_reader->GetImageOrientationPatient();
  std::cout << "Pos: " << position[0] << ", " <<  position[1] << ", " << position[2] << "\n";
  std::cout << "Orient: " << orientation[0] << ", " <<  orientation[1] << ", " << orientation[2] << "\n";
  //  std::cout << "Res: " << p_reader->GetWidth() << " * " << p_reader->GetHeight() << "\n";
  //
  // Annotate the image with the DICOM information...
  vtkTextMapper *p_textMapper = (vtkTextMapper *)p_imgPanel->p_dicomAnnotation->GetMapper();
  char dcmInfo[200];
  sprintf(dcmInfo, "Dim: %d x %d\nLoc: %5.1f, %5.1f, %5.1f", p_reader->GetWidth(), p_reader->GetHeight(),
      position[0], position[1], position[2]);
  p_textMapper->SetInput(dcmInfo);
}

// Set up the desired image panel for display...
void minirGui::initImgPanel(imgPanel *p_imgPanel)
{
  //
  // Initialize the image panel...
  vtkImageViewer2* p_imageViewer = vtkImageViewer2::New();
  p_imgPanel->p_imageViewer = p_imageViewer;
  p_imgPanel->p_widget->SetRenderWindow(p_imageViewer->GetRenderWindow());
  vtkRenderWindowInteractor *renderWindowInteractor =
    p_imgPanel->p_widget->GetRenderWindow()->GetInteractor();
  p_imageViewer->SetupInteractor(renderWindowInteractor);
  //
  // Add the image orientation annotation...
  if (p_imgPanel->annotation.size() > 0) {
    vtkSmartPointer<vtkCornerAnnotation> annotation =
      vtkSmartPointer<vtkCornerAnnotation>::New();
    annotation->SetText( 2, p_imgPanel->annotation.c_str());
    annotation->GetTextProperty()->SetColor( 1,0,0);
    p_imageViewer->GetRenderer()->AddViewProp(annotation);
  }
  //
  // Set up the DICOM image annotation...
  vtkSmartPointer<vtkTextProperty> annotationTextProp = vtkSmartPointer<vtkTextProperty>::New();
  annotationTextProp->SetFontFamilyToCourier();
  annotationTextProp->SetFontSize(14);
  annotationTextProp->SetVerticalJustificationToTop();
  annotationTextProp->SetJustificationToLeft();
  annotationTextProp->SetColor(1, 1, 0);
  //
  vtkSmartPointer<vtkTextMapper> annotationTextMapper = vtkSmartPointer<vtkTextMapper>::New();
  annotationTextMapper->SetTextProperty(annotationTextProp);
  //
  vtkSmartPointer<vtkActor2D> annotationTextActor = vtkSmartPointer<vtkActor2D>::New();
  p_imgPanel->p_dicomAnnotation = annotationTextActor;
  annotationTextActor->SetMapper(annotationTextMapper);
  annotationTextActor->GetPositionCoordinate()->SetCoordinateSystemToDisplay();
  annotationTextActor->GetPositionCoordinate()->SetValue(10, 35);
  //
  p_imgPanel->p_imageViewer->GetRenderer()->AddActor2D(annotationTextActor);
  //
  // Display a default image...
  std::string tf1 = "test.dcm";
  loadDicom(tf1, p_imgPanel);
}

minirGui::minirGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::minirGui)
{
    ui->setupUi(this);
    //
    // Initialize the coronal, sagittal, axial, and 3D image panels...
    imgPanel_cor.annotation = "Coronal";
    imgPanel_cor.p_widget = ui->qVTK_cor;
    initImgPanel(&imgPanel_cor);
    //
    imgPanel_sag.annotation = "Sagittal";
    imgPanel_sag.p_widget = ui->qVTK_sag;
    initImgPanel(&imgPanel_sag);
    //
    imgPanel_ax.annotation = "Axial";
    imgPanel_ax.p_widget = ui->qVTK_ax;
    initImgPanel(&imgPanel_ax);
    //
    imgPanel_3D.annotation = "3D";
    imgPanel_3D.p_widget = ui->qVTK_3D;
    initImgPanel(&imgPanel_3D);

    //
    // Set up to load images with a right click in the coronal window...
    Connections = vtkEventQtSlotConnect::New();
    Connections->Connect(ui->qVTK_cor->GetRenderWindow()->GetInteractor(),
                         vtkCommand::RightButtonPressEvent,
                         this,
                         SLOT(loadImages(vtkObject*)));
    Connections->Connect(ui->qVTK_sag->GetRenderWindow()->GetInteractor(),
                         vtkCommand::RightButtonPressEvent,
                         this,
                         SLOT(loadImages(vtkObject*)));
    Connections->Connect(ui->qVTK_ax->GetRenderWindow()->GetInteractor(),
                         vtkCommand::RightButtonPressEvent,
                         this,
                         SLOT(loadImages(vtkObject*)));
   }

minirGui::~minirGui()
{
    delete ui;
}

void minirGui::on_endoConnect_clicked()
{
    if (ui->endoConnect->isChecked()) {
        ui->statusText->appendPlainText("Connecting to EndoScout");
        int result = endoScout.endoConnect("192.168.2.5", 20248);
        if (!result) {
            ui->statusText->appendPlainText("  - EndoScout connected");
        } else {
            switch (result) {
            case -1:
                ui->statusText->appendPlainText("  - Unable to create socket");
                break;
            case -2:
                ui->statusText->appendPlainText("  - EndoScout not found");
                break;
            case -3:
                ui->statusText->appendPlainText("  - No response from EndoScout");
                break;
            case -4:
                ui->statusText->appendPlainText("  - Invalid EndoScout response");
                break;
            default:
                ui->statusText->appendPlainText("  - Huh???");
                break;
            }
            ui->endoConnect->setChecked(false);
        }
    } else {
        ui->statusText->appendPlainText("EndoScout Disconnected");
    }
}

void minirGui::on_test_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Pick a Directory", ".", QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    std::cout << "Directory: " << dir.toAscii().constData() << endl;
}

void minirGui::loadImages(vtkObject *)
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Pick some files", ".");
    for (int i = 0; i < files.size(); i++) {
        cout << "FIle: " << files.at(i).toAscii().constData() << endl;
    }
}

void minirGui::on_endoLog_clicked()
{

}

void minirGui::on_clearStatus_clicked()
{
    ui->statusText->clear();
}

