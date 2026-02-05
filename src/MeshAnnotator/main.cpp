#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>

#include "InteractorStyleCellPicker.h"
#include "InteractorStylePointPicker.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);
    MainWindow mainWindow;
    
    mainWindow.resize(1200, 900);

    mainWindow.show();
    
    return app.exec();
}
