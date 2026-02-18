#include "VtkViewport.h"

#include <QVBoxLayout>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkPLYReader.h>
#include <vtkNew.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>
#include <vtkExtractCells.h>
#include <vtkGeometryFilter.h>

#include "InteractorStyleCellPicker.h"
#include "InteractorStylePointPicker.h"

VtkViewport::VtkViewport(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);

    vtkWidget = new QVTKOpenGLNativeWidget(this);
    vtkWidget->setFocusPolicy(Qt::StrongFocus);

    layout->addWidget(vtkWidget);

    // Render window
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkWidget->setRenderWindow(renderWindow);

    // Renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.1, 0.2, 0.4);
    renderWindow->AddRenderer(renderer);
}

// void VtkViewport::setPickerMode(int mode)
// {
//     // currentPickerStyle = availablePickers.at(mode);
//     // currentPickerStyle->SetObservedActor(this->currentActor);
//     // // TODO : There.s a bug with the picker where it responds poorly before a zoom out/rotation is made
//     // vtkWidget->interactor()->SetInteractorStyle(currentPickerStyle);
//     // vtkWidget->setFocusPolicy(Qt::StrongFocus);
//     // vtkWidget->setFocus();
//     // std::cout << std::to_string(mode) << std::endl;
//     // // switch picker styles here later
// }

void VtkViewport::loadMesh(std::string path)
{
    auto reader = vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName(path.c_str());
    reader->Update();
    
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(reader->GetOutput());

    if (this->currentActor)
        renderer->RemoveAllViewProps();
    else
        this->currentActor = vtkSmartPointer<vtkActor>::New();

    this->currentActor->SetMapper(mapper);

    renderer->AddActor(this->currentActor);
}
