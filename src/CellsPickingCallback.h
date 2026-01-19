#pragma once

#include <vtkActor.h>
#include <vtkIdList.h>
#include <vtkDataSet.h>
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPoints.h>
#include <vtkMapper.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkCellPicker.h>


class CellsPickingCallback : public vtkCommand
{
public:
    static CellsPickingCallback* New()
    {
        return new CellsPickingCallback();
    }

    CellsPickingCallback()
    {   this->isSelecting = false;
        this->lastEventIsStartSelecting = false;
        this->cellIds = vtkSmartPointer<vtkIdList>::New();

        this->cellPicker = vtkSmartPointer<vtkCellPicker>::New();
        this->cellPicker->SetTolerance(0.01);
    }

    inline void SetObservedActor(vtkSmartPointer<vtkActor> actor) { this->observedActor = actor; }
    inline const vtkSmartPointer<vtkIdList> GetSelectedCellIds() { return this->cellIds;}
    const void RegisterInteractorObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
    {
        interactor->AddObserver(vtkCommand::LeftButtonPressEvent, this);
        interactor->AddObserver(vtkCommand::EndInteractionEvent, this);
        interactor->AddObserver(vtkCommand::MouseMoveEvent, this);
    }

    void Execute(vtkObject* caller, unsigned long eventId, void* callData) override
    {
        vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
        const int x = interactor->GetEventPosition()[0];
        const int y = interactor->GetEventPosition()[1];

        this->cellPicker->Pick(x, y, 0, interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        const vtkIdType cellId = this->cellPicker->GetCellId();


        if (eventId == vtkCommand::LeftButtonPressEvent)
        {
            this->isSelecting = true;
            std::cout << "selecting on" << std::endl;
            this->lastEventIsStartSelecting = true;
            return;
        }
        else if (eventId == vtkCommand::EndInteractionEvent && this->lastEventIsStartSelecting)
        {
            this->isSelecting = false;
            std::cout << "selecting stopped" << std::endl;
            this->lastEventIsStartSelecting = false;
            return;
        }
        else if (eventId == vtkCommand::MouseMoveEvent && this->isSelecting)
        {
            std::cout << "selecting..." << std::endl;
        }

        if (!this->isSelecting || cellId == -1)
            return;

        double pointPosition[3] = {0, 0, 0};
        this->observedActor->GetMapper()->GetInput()->GetPoint(cellId, pointPosition);

        if (this->cellIds->IsId(cellId) != -1)
            return;

        this->cellIds->InsertNextId(cellId);
        this->cellIds->Modified();
        std::cout << "selected cell id: " << cellId << std::endl;

    }
private:
    bool lastEventIsStartSelecting;
    bool isSelecting;
    vtkSmartPointer<vtkCellPicker> cellPicker;
    vtkSmartPointer<vtkPoints> landmarks;
    vtkSmartPointer<vtkIdList> cellIds;
    vtkSmartPointer<vtkActor> observedActor;
};