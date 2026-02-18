#pragma once

#include "NewPickerComboBox.h"

#include <QVBoxLayout>
#include <vtkInteractorStyleTrackballCamera.h>

class QWidget;

class ControlPanel: public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanel(QWidget* parent = nullptr);

signals:

private slots:
    void createPicker(const PickerType& type);

private:
    NewPickerComboBox* pickerTypeComboBox;
    QVBoxLayout* mainLayout;
};