#pragma once
#include "ControlPanel.h"
#include "VtkViewport.h"

#include <QMainWindow>
#include <QDockWidget>

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
signals:
    void meshLoaded(std::string path);
private slots:
    void openMesh();


private:

ControlPanel* controlPanel;
VtkViewport* vtkViewport;

QMenu* fileMenu;
QAction* openMeshAct;
};
