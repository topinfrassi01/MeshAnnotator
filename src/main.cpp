#include <iostream>

#include <vtkNew.h>
#include <vtkCommand.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCellPicker.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkPolyData.h>
#include <vtkPointPicker.h>

#include "InteractorStyleCellPicker.h"
#include "InteractorStylePointPicker.h"

int main()
{
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
    renderer->SetBackground(0.1, 0.2, 0.4); // dark blue

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(800, 600);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);

    vtkNew<InteractorStylePointPicker<vtkInteractorStyleTrackballCamera>> style;
    style->SetObservedActor(actor);
    interactor->SetInteractorStyle(style);

    // Testing for point picking
    auto glyphPolyData = vtkSmartPointer<vtkPolyData>::New();
    glyphPolyData->SetPoints(style->GetTrackedPoints().Get());

    auto glyphFilterPolyData = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    glyphFilterPolyData->SetInputData(glyphPolyData);
    glyphFilterPolyData->Update();

    auto glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    glyphMapper->SetInputConnection(glyphFilterPolyData->GetOutputPort());

    auto glyphActor = vtkSmartPointer<vtkActor>::New();
    glyphActor->SetMapper(glyphMapper);
    glyphActor->GetProperty()->SetPointSize(10);
    glyphActor->GetProperty()->SetColor(0, 255, 0);
    
    renderer->AddActor(glyphActor);

    // Testing for cell picking
    // vtkNew<InteractorStyleCellPicker<vtkInteractorStyleTrackballCamera>> style;
    // style->SetObservedActor(actor);
    // interactor->SetInteractorStyle(style);
    // vtkNew<vtkGeometryFilter> geometryFilter;
    // geometryFilter->SetInputConnection(style->GetExtractSelectedCells()->GetOutputPort());
    // geometryFilter->Update();

    // auto highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // highlightMapper->SetInputConnection(geometryFilter->GetOutputPort());
    // highlightMapper->Update(); 

    // auto selectedCellActor = vtkSmartPointer<vtkActor>::New();
    // selectedCellActor->SetMapper(highlightMapper);
    // selectedCellActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
    
    // renderer->AddActor(selectedCellActor);

    renderWindow->Render();
    interactor->Start();

    return 0;
}
