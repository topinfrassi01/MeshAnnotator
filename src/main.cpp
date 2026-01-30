#include <iostream>
#include <cstdlib>

#include <QVTKOpenGLNativeWidget.h>
#include <QApplication>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>

#include <vtkCallbackCommand.h>
#include <vtkNew.h>
#include <vtkDataSetMapper.h>
#include <vtkCommand.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCellPicker.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkPolyData.h>
#include <vtkPointPicker.h>

#include "InteractorStyleCellPicker.h"
#include "InteractorStylePointPicker.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);

    // Main window.
    QMainWindow mainWindow;
    mainWindow.resize(1200, 900);

    // Control area.
    QDockWidget controlDock;
    mainWindow.addDockWidget(Qt::LeftDockWidgetArea, &controlDock);

    QLabel controlDockTitle("Control Dock");
    controlDockTitle.setMargin(20);
    controlDock.setTitleBarWidget(&controlDockTitle);

    QPointer<QVBoxLayout> dockLayout = new QVBoxLayout();
    QWidget layoutContainer;
    layoutContainer.setLayout(dockLayout);
    controlDock.setWidget(&layoutContainer);

    QPushButton randomizeButton;
    randomizeButton.setText("Randomize");
    dockLayout->addWidget(&randomizeButton);

    // Render area.
    QPointer<QVTKOpenGLNativeWidget> vtkRenderWidget =
        new QVTKOpenGLNativeWidget();

    mainWindow.setCentralWidget(vtkRenderWidget);

    // VTK part.
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkRenderWidget->setRenderWindow(renderWindow.Get());

    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(30);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cone->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetObjectName("Model");
    actor->SetMapper(mapper);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.2, 0.4);

    renderWindow->AddRenderer(renderer);
    
    // // Testing for point picking
    
    // vtkNew<InteractorStylePointPicker> style;
    // style->SetObservedActor(actor);
    // vtkRenderWidget->interactor()->SetInteractorStyle(style);
    // auto glyphPolyData = vtkSmartPointer<vtkPolyData>::New();
    // glyphPolyData->SetPoints(style->GetPickedObject().Get());

    // auto glyphFilterPolyData = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    // glyphFilterPolyData->SetInputData(glyphPolyData);
    // glyphFilterPolyData->Update();

    // auto glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // glyphMapper->SetInputConnection(glyphFilterPolyData->GetOutputPort());

    // auto glyphActor = vtkSmartPointer<vtkActor>::New();
    // glyphActor->SetMapper(glyphMapper);
    // glyphActor->GetProperty()->SetPointSize(10);
    // glyphActor->GetProperty()->SetColor(0, 255, 0);

    // renderer->AddActor(glyphActor);


    //Testing for cell picking
    vtkNew<InteractorStyleCellPicker> style;
    style->SetObservedActor(actor);
    vtkRenderWidget->interactor()->SetInteractorStyle(style);
    
    vtkNew<vtkExtractCells> extractCells;
    extractCells->SetInputData(actor->GetMapper()->GetInput());

    vtkNew<vtkGeometryFilter> geometryFilter;
    geometryFilter->SetInputConnection(extractCells->GetOutputPort());
    geometryFilter->Update();

    auto highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    highlightMapper->SetInputConnection(geometryFilter->GetOutputPort());
    highlightMapper->Update();

    auto selectedCellActor = vtkSmartPointer<vtkActor>::New();
    selectedCellActor->SetMapper(highlightMapper);
    selectedCellActor->GetProperty()->SetColor(0.0, 1.0, 0.0);

    renderer->AddActor(selectedCellActor);

    auto callback = vtkSmartPointer<vtkCallbackCommand>::New();
    struct CallbackData
    {
        vtkExtractCells* extractCells;
        vtkIdList* idList;
    };
    CallbackData cbData{extractCells.Get(), style->GetPickedObject().Get()};
    callback->SetClientData(&cbData);
    //callback->SetClientData(style->GetPickedObject().Get());
    callback->SetCallback([](vtkObject* caller,
                            unsigned long eventId,
                            void* clientData,
                            void*)
    {
        auto callbackData = static_cast<CallbackData*>(clientData);
        
        callbackData->extractCells->SetCellList(callbackData->idList);
        callbackData->extractCells->Modified();
        // std::cout << "Picked cells: ";
        // for (vtkIdType i = 0; i < idList->GetNumberOfIds(); ++i)
        // {
        //     std::cout << idList->GetId(i) << " ";
        // }
        // std::cout << std::endl;
    });
    style->GetPickedObject()->AddObserver(vtkCommand::ModifiedEvent, callback);

    mainWindow.show();
    vtkRenderWidget->setFocusPolicy(Qt::StrongFocus);
    vtkRenderWidget->setFocus();
    return app.exec();


    // //renderWindow->Render();
    // //interactor->Start();
    // window->AddRenderer(renderer);
    // mainWindow.show();
    // return app.exec();
    // //return 0;
}
