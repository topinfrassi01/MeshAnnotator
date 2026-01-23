#pragma once

#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkInteractorStyle.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

template <class T>
class InteractorStylePointPicker : public T
{
public:
    static InteractorStylePointPicker<T>* New()
    {
        return new InteractorStylePointPicker();
    }

    void DeletePointId(const vtkIdType pointId, vtkSmartPointer<vtkPoints> points)
    {
        auto newPoints = vtkSmartPointer<vtkPoints>::New();
        for(vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
            if (i != pointId)
                newPoints->InsertNextPoint(points->GetPoint(i));
        std::cout << points->GetNumberOfPoints() << std::endl;
        points->ShallowCopy(newPoints);
        std::cout << points->GetNumberOfPoints() << std::endl;
    }

    vtkTypeMacro(InteractorStylePointPicker<T>, T);

    InteractorStylePointPicker<T>()
    {
        this->trackedPointIds = vtkSmartPointer<vtkIdList>::New();
        this->trackedPoints = vtkSmartPointer<vtkPoints>::New();
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
    inline const vtkSmartPointer<vtkPoints> GetTrackedPoints() { return this->trackedPoints; }

    inline void SetObservedActor(vtkSmartPointer<vtkActor> actor) {
        this->observedActor = actor;
        this->pointPicker->AddPickList(actor);
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
            T::OnMouseMove();
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

        T::OnMouseMove();
    }

    void OnLeftButtonUp() override
    {
        const vtkIdType pickedPointId = this->pointPicker->GetPointId();
        if (pickedPointId != -1 && this->Interactor->GetAltKey() && this->trackedPointIds->IsId(pickedPointId) == -1)
        {
            double* pickedPosition = this->pointPicker->GetPickPosition();
            this->trackedPoints->InsertNextPoint(pickedPosition);
            this->trackedPointIds->InsertNextId(pickedPointId);
            std::cout << "Picked point id: " << pickedPointId << std::endl;
            this->trackedPoints->Modified();
        }
        T::OnLeftButtonUp();
    }

    void OnRightButtonUp() override
    {
        const vtkIdType pickedPointId = this->pointPicker->GetPointId();
        if (pickedPointId != -1 && this->Interactor->GetAltKey())
        {
            const vtkIdType ptId = this->trackedPointIds->FindIdLocation(pickedPointId);
            std::cout << "Picked point id: " << pickedPointId << std::endl;
            std::cout << "Picked point id id: " << ptId << std::endl;
            this->trackedPointIds->DeleteId(pickedPointId);
            this->trackedPointIds->Squeeze();
            std::cout << this->trackedPointIds->GetNumberOfIds() << std::endl;
            DeletePointId(ptId, this->trackedPoints);
            this->trackedPoints->Modified();
        }
        T::OnRightButtonUp();
    }
private:
    vtkSmartPointer<vtkIdList> trackedPointIds;
    vtkSmartPointer<vtkPoints> trackedPoints;
    vtkSmartPointer<vtkActor> observedActor;

    vtkSmartPointer<vtkPointPicker> pointPicker;
    vtkSmartPointer<vtkPolyDataMapper> glyphMapper;
    vtkSmartPointer<vtkActor> glyphActor;
};