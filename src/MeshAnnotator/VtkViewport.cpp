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

    // Picking style
    currentPickerStyle = vtkSmartPointer<InteractorStylePointPicker>::New();
    vtkWidget->interactor()->SetInteractorStyle(currentPickerStyle);

    // TODO : To delete as visualisation is handled in the interactor now. Kept only to remember how to catch events.
    // struct CallbackData
    // {
    //     vtkExtractCells* extractCells;
    //     vtkIdList* idList;
    // };

    // vtkNew<vtkExtractCells> extractCells;
    // extractCells->SetInputData(actor->GetMapper()->GetInput());

    // vtkNew<vtkGeometryFilter> geometryFilter;
    // geometryFilter->SetInputConnection(extractCells->GetOutputPort());
    // geometryFilter->Update();

    // auto highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // highlightMapper->SetInputConnection(geometryFilter->GetOutputPort());
    // highlightMapper->Update();

    // auto selectedCellActor = vtkSmartPointer<vtkActor>::New();
    // selectedCellActor->SetMapper(highlightMapper);
    // selectedCellActor->GetProperty()->SetColor(0.0, 1.0, 0.0);

    // renderer->AddActor(selectedCellActor);
    // auto* cbData = new CallbackData{
    //     extractCells.Get(),
    //     style->GetPickedObject().Get()
    // };

    // vtkNew<vtkCallbackCommand> callback;
    // callback->SetClientData(cbData);
    // callback->SetCallback([](vtkObject*, unsigned long, void* clientData, void*)
    // {
    //     auto* data = static_cast<CallbackData*>(clientData);
    //     data->extractCells->SetCellList(data->idList);
    //     data->extractCells->Modified();
    // });

    //style->GetPickedObject()->AddObserver(vtkCommand::ModifiedEvent, callback);
}

void VtkViewport::setPickerMode(int mode)
{
    std::cout << std::to_string(mode) << std::endl;
    // switch picker styles here later
}

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

    currentPickerStyle->SetObservedActor(this->currentActor);
    renderer->AddActor(this->currentActor);
    std::cout << path;
}
