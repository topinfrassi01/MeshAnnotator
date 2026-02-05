#pragma once
#include "ControlPanel.h"
#include "VtkViewport.h"

#include <QMainWindow>
#include <QDockWidget>

class MainWindow: public QMainWindow
{
public:
    MainWindow(QWidget* parent = nullptr);
private:
ControlPanel* controlPanel;
VtkViewport* vtkViewport;
};
