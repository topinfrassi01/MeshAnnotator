#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);
    MainWindow mainWindow;
    
    mainWindow.resize(1200, 900);

    // QDockWidget controlDock;
    // mainWindow.addDockWidget(Qt::LeftDockWidgetArea, &controlDock);

    // QLabel controlDockTitle("Control Dock");
    // controlDockTitle.setMargin(20);
    // controlDock.setTitleBarWidget(&controlDockTitle);

    // QPointer<QVBoxLayout> dockLayout = new QVBoxLayout();
    // QWidget layoutContainer;
    // layoutContainer.setLayout(dockLayout);
    // controlDock.setWidget(&layoutContainer);

    // QComboBox pickerTypeComboBox;
    // pickerTypeComboBox.addItem("New picker...");

    // pickerTypeComboBox.addItem("Point Picker");
    // pickerTypeComboBox.addItem("Cell Picker");
    // dockLayout->addWidget(&pickerTypeComboBox);
    // QPushButton randomizeButton;
    // randomizeButton.setText("Randomize");
    // dockLayout->addWidget(&randomizeButton);

    // QPointer<QVTKOpenGLNativeWidget> vtkRenderWidget = new QVTKOpenGLNativeWidget();
    // mainWindow.setCentralWidget(vtkRenderWidget);

    // vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    // vtkRenderWidget->setRenderWindow(renderWindow.Get());

    // vtkNew<vtkConeSource> cone;
    // cone->SetHeight(3.0);
    // cone->SetRadius(1.0);
    // cone->SetResolution(30);

    // vtkNew<vtkPolyDataMapper> mapper;
    // mapper->SetInputConnection(cone->GetOutputPort());

    // vtkNew<vtkActor> actor;
    // actor->SetMapper(mapper);

    // vtkNew<vtkRenderer> renderer;
    // renderer->AddActor(actor);
    // renderer->SetBackground(0.1, 0.2, 0.4);

    // renderWindow->AddRenderer(renderer);
    
    // // // Testing for point picking
    
    // // vtkNew<InteractorStylePointPicker> style;
    // // style->SetObservedActor(actor);
    // // vtkRenderWidget->interactor()->SetInteractorStyle(style);
    // // auto glyphPolyData = vtkSmartPointer<vtkPolyData>::New();
    // // glyphPolyData->SetPoints(style->GetPickedObject().Get());

    // // auto glyphFilterPolyData = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    // // glyphFilterPolyData->SetInputData(glyphPolyData);
    // // glyphFilterPolyData->Update();

    // // auto glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // // glyphMapper->SetInputConnection(glyphFilterPolyData->GetOutputPort());

    // // auto glyphActor = vtkSmartPointer<vtkActor>::New();
    // // glyphActor->SetMapper(glyphMapper);
    // // glyphActor->GetProperty()->SetPointSize(10);
    // // glyphActor->GetProperty()->SetColor(0, 255, 0);

    // // renderer->AddActor(glyphActor);


    // //Testing for cell picking
    // vtkNew<InteractorStyleCellPicker> style;
    // style->SetObservedActor(actor);
    // vtkRenderWidget->interactor()->SetInteractorStyle(style);
    
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

    // auto callback = vtkSmartPointer<vtkCallbackCommand>::New();
    // struct CallbackData
    // {
    //     vtkExtractCells* extractCells;
    //     vtkIdList* idList;
    // };
    // CallbackData cbData{extractCells.Get(), style->GetPickedObject().Get()};
    // callback->SetClientData(&cbData);
    // callback->SetCallback([](vtkObject* caller,
    //                         unsigned long eventId,
    //                         void* clientData,
    //                         void*)
    // {
    //     auto callbackData = static_cast<CallbackData*>(clientData);
        
    //     callbackData->extractCells->SetCellList(callbackData->idList);
    //     callbackData->extractCells->Modified();
    // });

    // style->GetPickedObject()->AddObserver(vtkCommand::ModifiedEvent, callback);

    mainWindow.show();
    
    return app.exec();
}
