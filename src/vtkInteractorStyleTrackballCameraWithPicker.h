#pragma once

#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>

template <class T>
class vtkInteractorStyleTrackballCameraWithPicker : public vtkInteractorStyleTrackballCamera
{
public:
    static vtkInteractorStyleTrackballCameraWithPicker<T>* New()
    {
        return new vtkInteractorStyleTrackballCameraWithPicker();
    }

    vtkTypeMacro(vtkInteractorStyleTrackballCameraWithPicker<T>, vtkInteractorStyleTrackballCamera);

    vtkInteractorStyleTrackballCameraWithPicker<T>()
    {
        this->pickedObject = vtkSmartPointer<T>::New();
    }

    virtual void SetObservedActor(vtkSmartPointer<vtkActor> actor){
        this->observedActor = actor;
    }

    const vtkSmartPointer<T> GetPickedObject() const
    {
        return this->pickedObject;
    }

protected:
    vtkSmartPointer<T> pickedObject;
    vtkSmartPointer<vtkActor> observedActor;
};
