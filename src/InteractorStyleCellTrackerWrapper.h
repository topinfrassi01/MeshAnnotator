/*
    vtkInteractorStyleTrackballVertexGlyphActor.h
*/
#pragma once

#include <vtkSmartPointer.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyle.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkIdList.h>
#include <vtkExtractCells.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkGeometryFilter.h>

template <class T>
class InteractorStyleCellTrackerWrapper : public T
{
public:
    static InteractorStyleCellTrackerWrapper<T>* New()
    {
        return new InteractorStyleCellTrackerWrapper();
    }

    vtkTypeMacro(InteractorStyleCellTrackerWrapper<T>, T);

    InteractorStyleCellTrackerWrapper<T>()
    {
        this->cellPicker = vtkSmartPointer<vtkCellPicker>::New();
        this->cellPicker->SetTolerance(0.01);
        this->isInSelectionMode = false;

        this->highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->selectedCellActor = vtkSmartPointer<vtkActor>::New();
    }

    inline void SetObservedActor(vtkSmartPointer<vtkActor> actor) {
        this->observedActor = actor;
    }

    void OnKeyPress() override
    {
        std::string key = this->Interactor->GetKeySym();
        if (key == "c")
        {
            this->isInSelectionMode = !this->isInSelectionMode;
            std::cout << "selection mode : " << this->isInSelectionMode << std::endl;
        }
    }
    
    void Rotate() override
    {
        if (!this->isInSelectionMode)
            T::Rotate();
    }

    void OnMouseMove() override
    {
        const int x = this->Interactor->GetEventPosition()[0];
        const int y = this->Interactor->GetEventPosition()[1];

        this->FindPokedRenderer(x, y);
        this->cellPicker->Pick(x, y, 0, this->GetCurrentRenderer());

        const vtkIdType cellId = this->cellPicker->GetCellId();
        if (cellId == -1 || !this->isInSelectionMode)
        {
            this->GetCurrentRenderer()->RemoveActor(this->selectedCellActor);
            this->GetCurrentRenderer()->GetRenderWindow()->Render();
            T::OnMouseMove();
            return;
        }

        vtkNew<vtkIdList> cellIds;
        cellIds->InsertNextId(cellId);

        vtkNew<vtkExtractCells> extractCells;
        extractCells->SetInputData(this->observedActor->GetMapper()->GetInput());
        extractCells->SetCellList(cellIds);
        extractCells->Update();

        vtkNew<vtkGeometryFilter> geometryFilter;
        geometryFilter->SetInputConnection(extractCells->GetOutputPort());
        geometryFilter->Update();

        highlightMapper->SetInputConnection(geometryFilter->GetOutputPort());
        highlightMapper->Update(); 

        selectedCellActor->SetMapper(highlightMapper);
        selectedCellActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        
        this->GetCurrentRenderer()->AddActor(this->selectedCellActor);
        this->GetCurrentRenderer()->GetRenderWindow()->Render();
        
        T::OnMouseMove();
    }
private:
    bool isInSelectionMode;
    vtkSmartPointer<vtkActor> observedActor;
    vtkSmartPointer<vtkCellPicker> cellPicker;
    vtkSmartPointer<vtkActor> selectedCellActor;
    vtkSmartPointer<vtkPolyDataMapper> highlightMapper;
};