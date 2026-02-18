#pragma once

#include "vtkInteractorStyleTrackballCameraWithPicker.h"

#include <vtkExtractCells.h>
#include <vtkGeometryFilter.h>
#include <vtkCellPicker.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkIdList.h>
#include <vtkPolyDataMapper.h>

class InteractorStyleCellPicker : public vtkInteractorStyleTrackballCameraWithPicker
{
public:
    static InteractorStyleCellPicker* New()
    {
        return new InteractorStyleCellPicker();
    }

    vtkTypeMacro(InteractorStyleCellPicker, vtkInteractorStyleTrackballCameraWithPicker);

    InteractorStyleCellPicker();

    void SetObservedActor(vtkSmartPointer<vtkActor> actor) override;

    const void updateAnnotatedMesh(AnnotatedMesh& mesh) override;

    void OnKeyPress() override;

    void OnKeyRelease() override;

    void OnLeftButtonDown() override;

    void OnLeftButtonUp() override;

    void OnRightButtonDown() override;

    void OnRightButtonUp() override;
    
    void Rotate() override;

    void Dolly() override;

    void OnMouseMove() override;
private:
    void InsertPickedCellId(const vtkIdType& cellId);
    void RemovePickedCellId(const vtkIdType& cellId);
    void HighlightHoveredCell(const vtkIdType cellId);
    void HighlightSelectedCells();

    bool isInSelectionMode;
    bool isAdding;
    bool isDeleting;
    vtkSmartPointer<vtkIdList> pickedCells;
    vtkSmartPointer<vtkCellPicker> cellPicker;

    vtkSmartPointer<vtkActor> highlightedCellActor;
    vtkSmartPointer<vtkIdList> highlightedCellIdList;
    vtkSmartPointer<vtkPolyDataMapper> highlightedCellMapper;

    vtkSmartPointer<vtkExtractCells> extractSelectedCells;
    vtkSmartPointer<vtkGeometryFilter> geometryFilter;
    vtkSmartPointer<vtkPolyDataMapper> selectedCellsMapper;
    vtkSmartPointer<vtkActor> selectedCellsActor;
};