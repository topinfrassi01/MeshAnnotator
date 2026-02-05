#pragma once
#include <tuple>
#include <vector>
#include <QComboBox>
#include <QWidget>
#include <vtkInteractorStyleTrackballCamera.h>
//#include "vtkInteractorStyleTrackballCameraWithPicker.h"

class ControlPanel: public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanel(QWidget* parent = nullptr);

signals:

    void pickerModeChanged(int mode);

private:
    QComboBox* pickerTypeComboBox;
};