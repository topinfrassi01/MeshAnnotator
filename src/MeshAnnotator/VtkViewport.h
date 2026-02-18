#pragma once

#include <QWidget>
#include <vector>

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include "InteractorStylePointPicker.h"

class QVTKOpenGLNativeWidget;


class VtkViewport : public QWidget
{
    Q_OBJECT
public:
    explicit VtkViewport(QWidget* parent = nullptr);

signals:
    void pickedObjectChanged();

public slots:
    //void setCurrentPicker(int mode);
    //void addNewPicker();
    void loadMesh(std::string path);

private:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkActor> currentActor;
    // TODO : Need to remove template from those functions as it makes abstracting impossible.
    vtkSmartPointer<vtkInteractorStyleTrackballCameraWithPicker> currentPickerStyle;
    std::vector<vtkSmartPointer<vtkInteractorStyleTrackballCameraWithPicker>> availablePickers;
    QVTKOpenGLNativeWidget* vtkWidget;
};