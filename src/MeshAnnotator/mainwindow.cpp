#include "MainWindow.h"
#include "ControlPanel.h"
#include "VtkViewport.h"

#include <QDockWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(1200, 900);

    vtkViewport  = new VtkViewport(this);
    controlPanel = new ControlPanel(this);

    auto* dock = new QDockWidget("Controls", this);
    dock->setWidget(controlPanel);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    setCentralWidget(vtkViewport);

    connect(controlPanel, &ControlPanel::pickerModeChanged,
            vtkViewport,  &VtkViewport::setPickerMode);
}