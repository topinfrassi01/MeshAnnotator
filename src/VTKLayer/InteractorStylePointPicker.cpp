#include "InteractorStylePointPicker.h"

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

    InteractorStylePointPicker::InteractorStylePointPicker()
    {
        this->pickedPoints = vtkSmartPointer<vtkPoints>::New();
        this->trackedCellIds = vtkSmartPointer<vtkIdList>::New();
        this->pointPicker = vtkSmartPointer<vtkCellPicker>::New();
        this->pointPicker->SetTolerance(0.001);
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

    const void updateAnnotatedMesh(AnnotatedMesh& mesh)
    {
        //mesh.updatePointsAnnotations()
    }

    void InteractorStylePointPicker::SetObservedActor(vtkSmartPointer<vtkActor> actor) {
        vtkInteractorStyleTrackballCameraWithPicker::SetObservedActor(actor);
        this->pointPicker->InitializePickList();
        this->pointPicker->AddPickList(this->observedActor);

        this->selectedPointsData = vtkSmartPointer<vtkPolyData>::New();
        this->selectedPointsData->SetPoints(this->pickedPoints.Get());

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

        const vtkIdType& cellId = this->pointPicker->GetCellId();
        
        if (cellId == -1)
        {
            this->highlightedPointActor->VisibilityOff();
            this->GetCurrentRenderer()->GetRenderWindow()->Render();
            vtkInteractorStyleTrackballCameraWithPicker::OnMouseMove();
            return;
        }

        std::cout << cellId << std::endl;
        this->highlightedPointActor->VisibilityOn();
        double* pointLocation = this->pointPicker->GetPickPosition();
    
        vtkPoints *points = this->highlightedPointGlyphMapper->GetInput()->GetPoints();
        points->SetNumberOfPoints(1);

        points->SetPoint(0, pointLocation);
        this->highlightedPointGlyphMapper->Modified();

        this->GetCurrentRenderer()->AddActor(this->highlightedPointActor);
        this->GetCurrentRenderer()->GetRenderWindow()->Render();

        vtkInteractorStyleTrackballCameraWithPicker::OnMouseMove();
    }

    void InteractorStylePointPicker::OnLeftButtonUp()
    {
        const vtkIdType pickedCellId = this->pointPicker->GetCellId();
        
        if (pickedCellId != -1 && this->Interactor->GetAltKey() && this->trackedCellIds->IsId(pickedCellId) == -1)
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

            this->pickedPoints->InsertNextPoint(pickedPosition);
            this->trackedCellIds->InsertNextId(pickedCellId);
            this->pickedPoints->Modified();
        }
        vtkInteractorStyleTrackballCameraWithPicker::OnLeftButtonUp();
    }

    void InteractorStylePointPicker::OnRightButtonUp()
    {
        const vtkIdType pickedCellId = this->pointPicker->GetCellId();
        if (pickedCellId != -1 && this->Interactor->GetAltKey())
        {
            const vtkIdType ptId = this->trackedCellIds->FindIdLocation(pickedCellId);
            this->trackedCellIds->DeleteId(pickedCellId);
            this->trackedCellIds->Squeeze();
            
            DeletePointId(ptId, this->pickedPoints);
            this->pickedPoints->Modified();
        }
        vtkInteractorStyleTrackballCameraWithPicker::OnRightButtonUp();
    }

    void InteractorStylePointPicker::DeletePointId(const vtkIdType cellId, vtkSmartPointer<vtkPoints> points)
    {
        auto newPoints = vtkSmartPointer<vtkPoints>::New();
        for(vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
            if (i != cellId)
                newPoints->InsertNextPoint(points->GetPoint(i));

        points->ShallowCopy(newPoints);
    }
