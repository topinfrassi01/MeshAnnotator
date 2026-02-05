#include "ControlPanel.h"

#include <QVBoxLayout>
#include <QComboBox>

ControlPanel::ControlPanel(QWidget* parent): QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);

    pickerTypeComboBox = new QComboBox(this);
    
    // pickerTypeComboBox->addItem("New picker...");
    // pickerTypeComboBox->addItem("Point Picker");
    // pickerTypeComboBox->addItem("Cell Picker");

    layout->addWidget(pickerTypeComboBox);
    layout->addStretch();
    
    connect(pickerTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlPanel::pickerModeChanged);
}