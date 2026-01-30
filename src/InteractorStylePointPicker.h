#pragma once
#include "vtkInteractorStyleTrackballCameraWithPicker.h"

#include <vtkPointPicker.h>
#include <vtkInteractorStyle.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
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

    InteractorStylePointPicker()
    {
        this->trackedPointIds = vtkSmartPointer<vtkIdList>::New();
        this->pointPicker = vtkSmartPointer<vtkPointPicker>::New();
        this->pointPicker->SetTolerance(0.01);
        this->pointPicker->SetPickFromList(true);

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        points->InsertNextPoint(0, 0, 0);
        auto glyphPolyData = vtkSmartPointer<vtkPolyData>::New();
        glyphPolyData->SetPoints(points);

        auto glyphFilterPolyData = vtkSmartPointer<vtkVertexGlyphFilter>::New();
        glyphFilterPolyData->SetInputData(glyphPolyData);
        glyphFilterPolyData->Update();

        this->glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->glyphMapper->SetInputConnection(glyphFilterPolyData->GetOutputPort());

        this->glyphActor = vtkSmartPointer<vtkActor>::New();
        this->glyphActor->SetMapper(this->glyphMapper);
        this->glyphActor->GetProperty()->SetPointSize(10);
        this->glyphActor->GetProperty()->SetColor(255, 0, 0);
    }

    void SetObservedActor(vtkSmartPointer<vtkActor> actor) override {
        InteractorStylePointPickerBase::SetObservedActor(actor);
        this->pointPicker->AddPickList(this->observedActor);
    }

    void OnMouseMove() override
    {
        const int x = this->Interactor->GetEventPosition()[0];
        const int y = this->Interactor->GetEventPosition()[1];

        this->FindPokedRenderer(x, y);
        this->pointPicker->Pick(x, y, 0, this->GetCurrentRenderer());

        double *pickedPosition = this->pointPicker->GetPickPosition();
        const vtkIdType pointId = this->pointPicker->GetPointId();

        if (pointId == -1)
        {
            this->glyphActor->VisibilityOff();
            this->GetCurrentRenderer()->GetRenderWindow()->Render();
            InteractorStylePointPickerBase::OnMouseMove();
            return;
        }

        this->glyphActor->VisibilityOn();

        vtkPoints *points = this->glyphMapper->GetInput()->GetPoints();
        points->SetNumberOfPoints(1);

        points->SetPoint(0, pickedPosition);
        points->InsertNextPoint(pickedPosition);
        points->Modified();
        this->glyphMapper->GetInput()->Modified();

        this->GetCurrentRenderer()->AddActor(this->glyphActor);
        this->GetCurrentRenderer()->GetRenderWindow()->Render();

        InteractorStylePointPickerBase::OnMouseMove();
    }

    void OnLeftButtonUp() override
    {
        const vtkIdType pickedPointId = this->pointPicker->GetPointId();
        if (pickedPointId != -1 && this->Interactor->GetAltKey() && this->trackedPointIds->IsId(pickedPointId) == -1)
        {
            double* pickedPosition = this->pointPicker->GetPickPosition();

            this->pickedObject->InsertNextPoint(pickedPosition);
            this->trackedPointIds->InsertNextId(pickedPointId);
            this->pickedObject->Modified();
        }
        InteractorStylePointPickerBase::OnLeftButtonUp();
    }

    void OnRightButtonUp() override
    {
        const vtkIdType pickedPointId = this->pointPicker->GetPointId();
        if (pickedPointId != -1 && this->Interactor->GetAltKey())
        {
            const vtkIdType ptId = this->trackedPointIds->FindIdLocation(pickedPointId);
            this->trackedPointIds->DeleteId(pickedPointId);
            this->trackedPointIds->Squeeze();
            
            DeletePointId(ptId, this->pickedObject);
            this->pickedObject->Modified();
        }
        InteractorStylePointPickerBase::OnRightButtonUp();
    }
private:
    void DeletePointId(const vtkIdType pointId, vtkSmartPointer<vtkPoints> points)
    {
        auto newPoints = vtkSmartPointer<vtkPoints>::New();
        for(vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
            if (i != pointId)
                newPoints->InsertNextPoint(points->GetPoint(i));

        points->ShallowCopy(newPoints);
    }

private:
    vtkSmartPointer<vtkIdList> trackedPointIds;

    vtkSmartPointer<vtkPointPicker> pointPicker;
    vtkSmartPointer<vtkPolyDataMapper> glyphMapper;
    vtkSmartPointer<vtkActor> glyphActor;
};