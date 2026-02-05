#include "InteractorStylePointPicker.h"
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

    InteractorStylePointPicker::InteractorStylePointPicker()
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

        this->highlightedPointGlyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->highlightedPointGlyphMapper->SetInputConnection(glyphFilterPolyData->GetOutputPort());

        this->highlightedPointActor = vtkSmartPointer<vtkActor>::New();
        this->highlightedPointActor->SetMapper(this->highlightedPointGlyphMapper);
        this->highlightedPointActor->GetProperty()->SetPointSize(10);
        this->highlightedPointActor->GetProperty()->SetColor(255, 0, 0);
    }

    void InteractorStylePointPicker::SetObservedActor(vtkSmartPointer<vtkActor> actor) {
        InteractorStylePointPickerBase::SetObservedActor(actor);
        this->pointPicker->AddPickList(this->observedActor);

        this->selectedPointsData = vtkSmartPointer<vtkPolyData>::New();
        this->selectedPointsData->SetPoints(this->GetPickedObject().Get());

        auto glyphFilterPolyData = vtkSmartPointer<vtkVertexGlyphFilter>::New();
        glyphFilterPolyData->SetInputData(this->selectedPointsData);
        glyphFilterPolyData->Update();

        this->selectedPointsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->selectedPointsMapper->SetInputConnection(glyphFilterPolyData->GetOutputPort());
    }

    void InteractorStylePointPicker::OnMouseMove()
    {
        const int x = this->Interactor->GetEventPosition()[0];
        const int y = this->Interactor->GetEventPosition()[1];

        this->FindPokedRenderer(x, y);
        this->pointPicker->Pick(x, y, 0, this->GetCurrentRenderer());

        double *pickedPosition = this->pointPicker->GetPickPosition();
        const vtkIdType pointId = this->pointPicker->GetPointId();

        if (pointId == -1)
        {
            this->highlightedPointActor->VisibilityOff();
            this->GetCurrentRenderer()->GetRenderWindow()->Render();
            InteractorStylePointPickerBase::OnMouseMove();
            return;
        }

        this->highlightedPointActor->VisibilityOn();

        vtkPoints *points = this->highlightedPointGlyphMapper->GetInput()->GetPoints();
        points->SetNumberOfPoints(1);

        points->SetPoint(0, pickedPosition);
        points->InsertNextPoint(pickedPosition);
        this->highlightedPointGlyphMapper->Modified();

        this->GetCurrentRenderer()->AddActor(this->highlightedPointActor);
        this->GetCurrentRenderer()->GetRenderWindow()->Render();

        InteractorStylePointPickerBase::OnMouseMove();
    }

    void InteractorStylePointPicker::OnLeftButtonUp()
    {
        const vtkIdType pickedPointId = this->pointPicker->GetPointId();
        if (pickedPointId != -1 && this->Interactor->GetAltKey() && this->trackedPointIds->IsId(pickedPointId) == -1)
        {
            if (!this->selectedPointsActor)
            {               
                this->selectedPointsActor = vtkSmartPointer<vtkActor>::New();
                this->selectedPointsActor->SetMapper(this->selectedPointsMapper);
                this->selectedPointsActor->GetProperty()->SetPointSize(10);
                this->selectedPointsActor->GetProperty()->SetColor(0, 255, 0);
                this->GetCurrentRenderer()->AddActor(this->selectedPointsActor);
            }
            double* pickedPosition = this->pointPicker->GetPickPosition();

            this->pickedObject->InsertNextPoint(pickedPosition);
            this->trackedPointIds->InsertNextId(pickedPointId);
            this->pickedObject->Modified();
        }
        InteractorStylePointPickerBase::OnLeftButtonUp();
    }

    void InteractorStylePointPicker::OnRightButtonUp()
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

    void InteractorStylePointPicker::DeletePointId(const vtkIdType pointId, vtkSmartPointer<vtkPoints> points)
    {
        auto newPoints = vtkSmartPointer<vtkPoints>::New();
        for(vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
            if (i != pointId)
                newPoints->InsertNextPoint(points->GetPoint(i));

        points->ShallowCopy(newPoints);
    }
