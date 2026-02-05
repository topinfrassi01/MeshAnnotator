#include "InteractorStyleCellPicker.h"

#include "vtkInteractorStyleTrackballCameraWithPicker.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyle.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkIdList.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkExtractCells.h>
#include <vtkPolyDataMapper.h>
#include <vtkGeometryFilter.h>

using InteractorStyleCellPickerBase = vtkInteractorStyleTrackballCameraWithPicker<vtkIdList>;

InteractorStyleCellPicker::InteractorStyleCellPicker()
        : isInSelectionMode(false), isAdding(false), isDeleting(false)
    {
        this->cellPicker = vtkSmartPointer<vtkCellPicker>::New();
        this->cellPicker->SetPickFromList(true);
        this->cellPicker->SetTolerance(0.01);

        this->highlightedCellIdList = vtkSmartPointer<vtkIdList>::New();
        this->highlightedCellIdList->SetNumberOfIds(1);

        this->highlightedCellMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->highlightedCellActor = vtkSmartPointer<vtkActor>::New();
    }

void InteractorStyleCellPicker::SetObservedActor(vtkSmartPointer<vtkActor> actor) {
    InteractorStyleCellPickerBase::SetObservedActor(actor);
    //this->extractSelectedCells->SetInputData(this->observedActor->GetMapper()->GetInput());
    this->cellPicker->AddPickList(this->observedActor);
}

void InteractorStyleCellPicker::OnKeyPress()
{
    InteractorStyleCellPickerBase::OnKeyPress();
    // Avoid changing isInSelectionMode if state is already frozen as OnKeyPress launches multiples times if held down
    if (this->isInSelectionMode)
        return;

    std::string key = this->Interactor->GetKeySym();
    if (key == "Alt_L" && !this->isInSelectionMode)
        this->isInSelectionMode = true;
}

void InteractorStyleCellPicker::OnKeyRelease()
{
    std::string key = this->Interactor->GetKeySym();
    if (key == "Alt_L")
        this->isInSelectionMode = false;
        
    InteractorStyleCellPickerBase::OnKeyRelease();
}

void InteractorStyleCellPicker::OnLeftButtonDown()
{
    if (isInSelectionMode)
        this->isAdding = true;

    InteractorStyleCellPickerBase::OnLeftButtonDown();
}

void InteractorStyleCellPicker::OnLeftButtonUp()
{
    if (this->isAdding)
    {
        const vtkIdType cellId = this->cellPicker->GetCellId();
        InsertPickedCellId(cellId);
    }

    this->isAdding = false;

    InteractorStyleCellPickerBase::OnLeftButtonUp();
}

void InteractorStyleCellPicker::OnRightButtonDown()
{
    if (isInSelectionMode)
        this->isDeleting = true;

    InteractorStyleCellPickerBase::OnRightButtonDown();
}

void InteractorStyleCellPicker::OnRightButtonUp()
{
    if (this->isDeleting)
    {
        const vtkIdType cellId = this->cellPicker->GetCellId();
        RemovePickedCellId(cellId);
    }
    this->isDeleting = false;
    InteractorStyleCellPickerBase::OnRightButtonUp();
}

void InteractorStyleCellPicker::Rotate()
{
    if (!this->isInSelectionMode)
        InteractorStyleCellPickerBase::Rotate();
}

void InteractorStyleCellPicker::Dolly()
{
    if (!this->isInSelectionMode)
        InteractorStyleCellPickerBase::Dolly();
}

void InteractorStyleCellPicker::OnMouseMove()
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

    HighlightHoveredCell(cellId);

    if (this->isAdding)
        InsertPickedCellId(cellId);
    else if (this->isDeleting)
        RemovePickedCellId(cellId);

    InteractorStyleCellPickerBase::OnMouseMove();
}

void InteractorStyleCellPicker::InsertPickedCellId(const vtkIdType& cellId)
{
    if (cellId == -1 || this->pickedObject->IsId(cellId) >= 0)
        return;

    this->pickedObject->InsertNextId(cellId);
    this->pickedObject->Modified();
}

void InteractorStyleCellPicker::RemovePickedCellId(const vtkIdType& cellId)
{
    if (cellId == -1 || this->pickedObject->IsId(cellId) == -1)
        return;

    this->pickedObject->DeleteId(cellId);
    this->pickedObject->Modified();
}

void InteractorStyleCellPicker::HighlightSelectedCells()
{

}

void InteractorStyleCellPicker::HighlightHoveredCell(const vtkIdType cellId)
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