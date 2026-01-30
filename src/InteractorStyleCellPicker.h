#pragma once

#include "vtkInteractorStyleTrackballCameraWithPicker.h"

#include <vtkCellPicker.h>
#include <vtkInteractorStyle.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkIdList.h>
#include <vtkExtractCells.h>
#include <vtkPolyDataMapper.h>
#include <vtkGeometryFilter.h>

using InteractorStyleCellPickerBase = vtkInteractorStyleTrackballCameraWithPicker<vtkIdList>;

class InteractorStyleCellPicker : public InteractorStyleCellPickerBase
{
public:
    static InteractorStyleCellPicker* New()
    {
        return new InteractorStyleCellPicker();
    }

    vtkTypeMacro(InteractorStyleCellPicker, InteractorStyleCellPickerBase);

    InteractorStyleCellPicker()
        : isInSelectionMode(false), isAdding(false), isDeleting(false)
    {
        this->extractSelectedCells = vtkSmartPointer<vtkExtractCells>::New();

        this->cellPicker = vtkSmartPointer<vtkCellPicker>::New();
        this->cellPicker->SetPickFromList(true);
        this->cellPicker->SetTolerance(0.01);

        this->highlightedCellIdList = vtkSmartPointer<vtkIdList>::New();
        this->highlightedCellIdList->SetNumberOfIds(1);

        this->highlightedCellMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->highlightedCellActor = vtkSmartPointer<vtkActor>::New();
    }

    void SetObservedActor(vtkSmartPointer<vtkActor> actor) override {
        InteractorStyleCellPickerBase::SetObservedActor(actor);
        //this->extractSelectedCells->SetInputData(this->observedActor->GetMapper()->GetInput());
        this->cellPicker->AddPickList(this->observedActor);
    }

    void OnKeyPress() override
    {
        InteractorStyleCellPickerBase::OnKeyPress();
        // Avoid changing isInSelectionMode if state is already frozen as OnKeyPress launches multiples times if held down
        if (this->isInSelectionMode)
            return;

        std::string key = this->Interactor->GetKeySym();
        if (key == "Alt_L" && !this->isInSelectionMode)
            this->isInSelectionMode = true;
    }

    void OnKeyRelease() override
    {
        std::string key = this->Interactor->GetKeySym();
        if (key == "Alt_L")
            this->isInSelectionMode = false;
            
        InteractorStyleCellPickerBase::OnKeyRelease();
    }

    void OnLeftButtonDown() override
    {
        if (isInSelectionMode)
            this->isAdding = true;

        InteractorStyleCellPickerBase::OnLeftButtonDown();
    }

    void OnLeftButtonUp() override
    {
        if (this->isAdding)
        {
            const vtkIdType cellId = this->cellPicker->GetCellId();
            InsertPickedCellId(cellId);
        }

        this->isAdding = false;

        InteractorStyleCellPickerBase::OnLeftButtonUp();
    }

    void OnRightButtonDown() override
    {
        if (isInSelectionMode)
            this->isDeleting = true;

        InteractorStyleCellPickerBase::OnRightButtonDown();
    }

    void OnRightButtonUp() override
    {
        if (this->isDeleting)
        {
            const vtkIdType cellId = this->cellPicker->GetCellId();
            RemovePickedCellId(cellId);
        }
        this->isDeleting = false;
        InteractorStyleCellPickerBase::OnRightButtonUp();
    }
    
    void Rotate() override
    {
        if (!this->isInSelectionMode)
            InteractorStyleCellPickerBase::Rotate();
    }

    void Dolly() override
    {
        if (!this->isInSelectionMode)
            InteractorStyleCellPickerBase::Dolly();
    }

    void OnMouseMove() override
    {
        const int x = this->Interactor->GetEventPosition()[0];
        const int y = this->Interactor->GetEventPosition()[1];

        this->FindPokedRenderer(x, y);
        this->cellPicker->Pick(x, y, 0, this->GetCurrentRenderer());

        const vtkIdType cellId = this->cellPicker->GetCellId();
        if (cellId == -1)
        {
            this->GetCurrentRenderer()->RemoveActor(this->highlightedCellActor);
            this->GetCurrentRenderer()->GetRenderWindow()->Render();
            InteractorStyleCellPickerBase::OnMouseMove();
            return;
        }

        HighlightSelectedCell(cellId);

        if (this->isAdding)
            InsertPickedCellId(cellId);
        else if (this->isDeleting)
            RemovePickedCellId(cellId);

        InteractorStyleCellPickerBase::OnMouseMove();
    }

    //inline const vtkSmartPointer<vtkExtractCells> GetExtractSelectedCells() { return this->extractSelectedCells; }
private:
    void InsertPickedCellId(const vtkIdType& cellId)
    {
        if (cellId == -1 || this->pickedObject->IsId(cellId) >= 0)
            return;

        this->pickedObject->InsertNextId(cellId);
        this->pickedObject->Modified();
        // We need to do this this way becayse vtkIdList doesn't fire "Modified" event.
        extractSelectedCells->SetCellList(this->pickedObject);
        extractSelectedCells->Modified();
    }

    void RemovePickedCellId(const vtkIdType& cellId)
    {
        if (cellId == -1 || this->pickedObject->IsId(cellId) == -1)
            return;

        this->pickedObject->DeleteId(cellId);
        this->pickedObject->Modified();
        // We need to do this this way becayse vtkIdList doesn't fire "Modified" event.
        extractSelectedCells->SetCellList(this->pickedObject);
        extractSelectedCells->Modified();
    }

    void HighlightSelectedCell(const vtkIdType cellId)
    {
        highlightedCellIdList->SetId(0, cellId);

        vtkNew<vtkExtractCells> extractCells;
        extractCells->SetAssumeSortedAndUniqueIds(true);
        extractCells->SetInputData(this->observedActor->GetMapper()->GetInput());
        extractCells->SetCellList(highlightedCellIdList);
        extractCells->Update();

        vtkNew<vtkGeometryFilter> geometryFilter;
        geometryFilter->SetInputConnection(extractCells->GetOutputPort());
        geometryFilter->Update();

        highlightedCellMapper->SetInputConnection(geometryFilter->GetOutputPort());
        highlightedCellMapper->Update();

        highlightedCellActor->SetMapper(highlightedCellMapper);
        highlightedCellActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

        this->GetCurrentRenderer()->AddActor(this->highlightedCellActor);
        this->GetCurrentRenderer()->GetRenderWindow()->Render();
    }

private:
    bool isInSelectionMode;
    bool isAdding;
    bool isDeleting;

    vtkSmartPointer<vtkExtractCells> extractSelectedCells;
    vtkSmartPointer<vtkCellPicker> cellPicker;

    vtkSmartPointer<vtkActor> highlightedCellActor;
    vtkSmartPointer<vtkIdList> highlightedCellIdList;
    vtkSmartPointer<vtkPolyDataMapper> highlightedCellMapper;
};