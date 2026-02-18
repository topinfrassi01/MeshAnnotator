#include "ControlPanel.h"

#include <QComboBox>

#include "PointPickerConfigurationTool.h"

ControlPanel::ControlPanel(QWidget* parent): QWidget(parent)
{
    pickerTypeComboBox = new NewPickerComboBox(this);
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(pickerTypeComboBox);   
    mainLayout->addStretch();
    
    connect(pickerTypeComboBox, &NewPickerComboBox::selectedPickerChanged,
            this, &ControlPanel::createPicker);

    setLayout(mainLayout);
}

void ControlPanel::createPicker(const PickerType& type)
{
    QWidget* newPicker;
    switch (type)
    {
        case PickerType::Points:
            newPicker = new PointPickerConfigurationTool(this);
        break;
        case PickerType::Cells:
        break;
    }
    mainLayout->addWidget(newPicker);
}