#pragma once

#include "vtkInteractorStyleTrackballCameraWithPicker.h"

#include <vtkCellPicker.h>
#include <vtkInteractorStyle.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>

class InteractorStylePointPicker : public vtkInteractorStyleTrackballCameraWithPicker
{
public:
    static InteractorStylePointPicker* New()
    {
        return new InteractorStylePointPicker();
    }
    vtkTypeMacro(InteractorStylePointPicker, vtkInteractorStyleTrackballCameraWithPicker);

    InteractorStylePointPicker();

    void SetObservedActor(vtkSmartPointer<vtkActor> actor) override;
    void OnMouseMove() override;
    void OnLeftButtonUp() override;
    void OnRightButtonUp() override;
    const void updateAnnotatedMesh(AnnotatedMesh& mesh) override;
private:
    void DeletePointId(const vtkIdType pointId, vtkSmartPointer<vtkPoints> points);

    vtkSmartPointer<vtkPoints> pickedPoints;
    vtkSmartPointer<vtkIdList> trackedCellIds;
    vtkSmartPointer<vtkCellPicker> pointPicker;

    vtkSmartPointer<vtkPolyDataMapper> highlightedPointGlyphMapper;
    vtkSmartPointer<vtkActor> highlightedPointActor;

    vtkSmartPointer<vtkPolyData> selectedPointsData;
    vtkSmartPointer<vtkPolyDataMapper> selectedPointsMapper;
    vtkSmartPointer<vtkActor> selectedPointsActor;
};