#pragma once

#include "vtkInteractorStyleTrackballCameraWithPicker.h"

#include <vtkPointPicker.h>
#include <vtkInteractorStyle.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>

using InteractorStylePointPickerBase = vtkInteractorStyleTrackballCameraWithPicker<vtkPoints>;

class InteractorStylePointPicker : public InteractorStylePointPickerBase
{
public:
    static InteractorStylePointPicker* New()
    {
        return new InteractorStylePointPicker();
    }
    vtkTypeMacro(InteractorStylePointPicker, InteractorStylePointPickerBase);

    InteractorStylePointPicker();

    void SetObservedActor(vtkSmartPointer<vtkActor> actor) override;
    void OnMouseMove() override;
    void OnLeftButtonUp() override;
    void OnRightButtonUp() override;
private:
    void DeletePointId(const vtkIdType pointId, vtkSmartPointer<vtkPoints> points);

    vtkSmartPointer<vtkIdList> trackedPointIds;
    vtkSmartPointer<vtkPointPicker> pointPicker;

    vtkSmartPointer<vtkPolyDataMapper> highlightedPointGlyphMapper;
    vtkSmartPointer<vtkActor> highlightedPointActor;

    vtkSmartPointer<vtkPolyData> selectedPointsData;
    vtkSmartPointer<vtkPolyDataMapper> selectedPointsMapper;
    vtkSmartPointer<vtkActor> selectedPointsActor;
};