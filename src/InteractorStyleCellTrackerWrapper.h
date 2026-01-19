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
        : isInSelectionMode(false), isSelecting(false)
    {
        this->extractSelectedCells = vtkSmartPointer<vtkExtractCells>::New();
        this->selectedCellIds = vtkSmartPointer<vtkIdList>::New();
        this->cellPicker = vtkSmartPointer<vtkCellPicker>::New();
        this->cellPicker->SetPickFromList(true);
        this->cellPicker->SetTolerance(0.01);

        this->highlightedCellIdList = vtkSmartPointer<vtkIdList>::New();
        this->highlightedCellIdList->SetNumberOfIds(1);

        this->highlightedCellMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->highlightedCellActor = vtkSmartPointer<vtkActor>::New();
    }

    inline void SetObservedActor(vtkSmartPointer<vtkActor> actor) {
        this->observedActor = actor;
        this->extractSelectedCells->SetInputData(this->observedActor->GetMapper()->GetInput());
        this->cellPicker->AddPickList(this->observedActor);
    }

    inline vtkSmartPointer<vtkIdList> GetSelectedCellIds() { return this->selectedCellIds; }
    inline const vtkSmartPointer<vtkExtractCells> GetExtractSelectedCells() { return this->extractSelectedCells; }
    void InsertPickedCellId(const vtkIdType& cellId)
    {
        if (cellId == -1 || this->selectedCellIds->IsId(cellId) >= 0)
            return;

        this->selectedCellIds->InsertNextId(cellId);

        // We need to do this this way becayse vtkIdList doesn't fire "Modified" event.
        extractSelectedCells->SetCellList(this->selectedCellIds);
        extractSelectedCells->Modified();

        std::cout << "Selected cell ID: " << cellId << std::endl;
    }

    void OnKeyPress() override
    {
        T::OnKeyPress();
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
            
        T::OnKeyRelease();
    }

    void OnLeftButtonDown() override
    {
        if (isInSelectionMode)
            this->isSelecting = true;

        T::OnLeftButtonDown();
    }

    void OnLeftButtonUp() override
    {
        if (this->isSelecting)
        {
            const vtkIdType cellId = this->cellPicker->GetCellId();
            InsertPickedCellId(cellId);
        }

        this->isSelecting = false;

        T::OnLeftButtonUp();
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
        if (cellId == -1)
        {
            this->GetCurrentRenderer()->RemoveActor(this->highlightedCellActor);
            this->GetCurrentRenderer()->GetRenderWindow()->Render();
            T::OnMouseMove();
            return;
        }

        HighlightSelectedCell(cellId);

        if (this->isSelecting)
            InsertPickedCellId(cellId);

        T::OnMouseMove();
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
    bool isSelecting;

    vtkSmartPointer<vtkExtractCells> extractSelectedCells;
    vtkSmartPointer<vtkIdList> selectedCellIds;
    vtkSmartPointer<vtkActor> observedActor;
    vtkSmartPointer<vtkCellPicker> cellPicker;

    vtkSmartPointer<vtkActor> highlightedCellActor;
    vtkSmartPointer<vtkIdList> highlightedCellIdList;
    vtkSmartPointer<vtkPolyDataMapper> highlightedCellMapper;
};