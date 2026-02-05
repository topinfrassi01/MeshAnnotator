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
    void setPickerMode(int mode);
    void loadMesh(std::string path);

private:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkActor> currentActor;
    // TODO : Need to remove template from those functions as it makes abstracting impossible.
    vtkSmartPointer<InteractorStylePointPicker> currentPickerStyle;
    QVTKOpenGLNativeWidget* vtkWidget;
};