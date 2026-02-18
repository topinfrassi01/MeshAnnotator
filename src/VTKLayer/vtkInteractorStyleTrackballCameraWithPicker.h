#pragma once

#include "AnnotatedMesh.h"
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>

class vtkInteractorStyleTrackballCameraWithPicker : public vtkInteractorStyleTrackballCamera
{
public:

    vtkInteractorStyleTrackballCameraWithPicker()
    {
    }

    virtual void SetObservedActor(vtkSmartPointer<vtkActor> actor){
        this->observedActor = actor;
    }

    const virtual void updateAnnotatedMesh(AnnotatedMesh& mesh) = 0;

protected:
    vtkSmartPointer<vtkActor> observedActor;
};
