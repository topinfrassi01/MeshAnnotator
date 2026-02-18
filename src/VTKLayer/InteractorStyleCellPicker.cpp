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

InteractorStyleCellPicker::InteractorStyleCellPicker()
        : isInSelectionMode(false), isAdding(false), isDeleting(false)
{
    this->pickedCells = vtkSmartPointer<vtkIdList>::New(); 
    this->cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    this->cellPicker->SetPickFromList(true);
    this->cellPicker->SetTolerance(0.001);

    this->highlightedCellIdList = vtkSmartPointer<vtkIdList>::New();
    this->highlightedCellIdList->SetNumberOfIds(1);

    this->highlightedCellMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    this->highlightedCellActor = vtkSmartPointer<vtkActor>::New();
}

const void InteractorStyleCellPicker::updateAnnotatedMesh(AnnotatedMesh& mesh)
{

}

void InteractorStyleCellPicker::SetObservedActor(vtkSmartPointer<vtkActor> actor) {
    vtkInteractorStyleTrackballCameraWithPicker::SetObservedActor(actor);
    this->cellPicker->AddPickList(this->observedActor);

    this->extractSelectedCells = vtkSmartPointer<vtkExtractCells>::New();
    this->extractSelectedCells->SetInputData(this->observedActor->GetMapper()->GetInput());
    this->extractSelectedCells->SetCellList(this->pickedCells);
    this->extractSelectedCells->Update();
    this->geometryFilter = vtkSmartPointer<vtkGeometryFilter>::New();
    this->geometryFilter->SetInputConnection(this->extractSelectedCells->GetOutputPort());
    this->geometryFilter->Update();
    this->selectedCellsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    this->selectedCellsMapper->SetInputConnection(this->geometryFilter->GetOutputPort());
    this->selectedCellsMapper->Update();
}

void InteractorStyleCellPicker::OnKeyPress()
{
    vtkInteractorStyleTrackballCameraWithPicker::OnKeyPress();
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
        
    vtkInteractorStyleTrackballCameraWithPicker::OnKeyRelease();
}

void InteractorStyleCellPicker::OnLeftButtonDown()
{
    if (isInSelectionMode)
        this->isAdding = true;

    vtkInteractorStyleTrackballCameraWithPicker::OnLeftButtonDown();
}

void InteractorStyleCellPicker::OnLeftButtonUp()
{
    if (this->isAdding)
    {
        const vtkIdType cellId = this->cellPicker->GetCellId();
        InsertPickedCellId(cellId);
    }

    this->isAdding = false;

    vtkInteractorStyleTrackballCameraWithPicker::OnLeftButtonUp();
}

void InteractorStyleCellPicker::OnRightButtonDown()
{
    if (isInSelectionMode)
        this->isDeleting = true;

    //InteractorStyleCellPickerBase::OnRightButtonDown();
}

void InteractorStyleCellPicker::OnRightButtonUp()
{
    if (this->isDeleting)
    {
        const vtkIdType cellId = this->cellPicker->GetCellId();
        RemovePickedCellId(cellId);
    }
    this->isDeleting = false;
    //InteractorStyleCellPickerBase::OnRightButtonUp();
}

void InteractorStyleCellPicker::Rotate()
{
    if (!this->isInSelectionMode)
        vtkInteractorStyleTrackballCameraWithPicker::Rotate();
}

void InteractorStyleCellPicker::Dolly()
{
    if (!this->isInSelectionMode)
        vtkInteractorStyleTrackballCameraWithPicker::Dolly();
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
        vtkInteractorStyleTrackballCameraWithPicker::OnMouseMove();
        return;
    }

    if (this->isAdding)
        InsertPickedCellId(cellId);
    else if (this->isDeleting)
        RemovePickedCellId(cellId);

    HighlightHoveredCell(cellId);

    vtkInteractorStyleTrackballCameraWithPicker::OnMouseMove();
}

void InteractorStyleCellPicker::InsertPickedCellId(const vtkIdType& cellId)
{
    if (cellId == -1 || this->pickedCells->IsId(cellId) >= 0)
        return;

    if (!this->selectedCellsActor)
    {
        this->selectedCellsActor = vtkSmartPointer<vtkActor>::New();
        this->selectedCellsActor->SetMapper(this->selectedCellsMapper);
        this->selectedCellsActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
        this->GetCurrentRenderer()->AddActor(this->selectedCellsActor);
    }

    this->pickedCells->InsertNextId(cellId);
    this->pickedCells->Modified();
    this->extractSelectedCells->SetCellList(this->pickedCells.Get());
    this->extractSelectedCells->Modified();
}

void InteractorStyleCellPicker::RemovePickedCellId(const vtkIdType& cellId)
{
    if (cellId == -1 || this->pickedCells->IsId(cellId) == -1)
        return;

    this->pickedCells->DeleteId(cellId);
    this->pickedCells->Modified();
    this->extractSelectedCells->SetCellList(this->pickedCells.Get());
    this->extractSelectedCells->Modified();
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

    // TODO : Would like the highlight to be rendered above the selected cells.
    highlightedCellMapper->SetInputConnection(geometryFilter->GetOutputPort());
    highlightedCellMapper->Update();

    highlightedCellActor->SetMapper(highlightedCellMapper);
    highlightedCellActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    this->GetCurrentRenderer()->AddActor(this->highlightedCellActor);
    this->GetCurrentRenderer()->GetRenderWindow()->Render();
    
}