#include "MainWindow.h"
#include "ControlPanel.h"
#include "VtkViewport.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QDockWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(1200, 900);

    vtkViewport  = new VtkViewport(this);
    controlPanel = new ControlPanel(this);

    fileMenu = menuBar()->addMenu(tr("File"));
    openMeshAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), tr("&Load mesh"), this);
    openMeshAct->setStatusTip(tr("Loads a mesh"));
    fileMenu->addAction(openMeshAct);
    connect(openMeshAct, &QAction::triggered, this, &MainWindow::openMesh);

    auto* dock = new QDockWidget("Controls", this);
    dock->setWidget(controlPanel);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    setCentralWidget(vtkViewport);

    connect(controlPanel, &ControlPanel::pickerModeChanged,
            vtkViewport,  &VtkViewport::setPickerMode);

    connect(this, &MainWindow::meshLoaded, vtkViewport, &VtkViewport::loadMesh);
}

void MainWindow::openMesh()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load mesh"), tr(""), tr("Mesh Files (*.ply *.stl *.vtk *.obj)"));
    emit meshLoaded(filename.toStdString());
}