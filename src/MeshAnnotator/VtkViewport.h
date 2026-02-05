#pragma once

#include <QWidget>
#include <vector>

#include <vtkSmartPointer.h>

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

private:
    QVTKOpenGLNativeWidget* vtkWidget;
};