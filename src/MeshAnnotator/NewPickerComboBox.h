#pragma once

#include <QWidget>
#include <QAction>
#include <QVBoxLayout>
#include <QComboBox>

enum PickerType
{
    None = 0,
    Points,
    Cells
};

class NewPickerComboBox: public QWidget
{
    Q_OBJECT
public:
    NewPickerComboBox(QWidget* parent): QWidget(parent)
    {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        comboBox = new QComboBox(this);
        
        comboBox->addItem("New picker...", PickerType::None);
        comboBox->addItem("Points", PickerType::Points);
        comboBox->addItem("Cells", PickerType::Cells);

        mainLayout->addWidget(comboBox);
        setLayout(mainLayout);

        connect(comboBox, &QComboBox::currentIndexChanged, this, &NewPickerComboBox::indexChanged);
    }

signals:
void selectedPickerChanged(const PickerType& type);

private slots:
void indexChanged(int index)
{
    PickerType type = static_cast<PickerType>(comboBox->currentData().toInt());
    if (type == PickerType::None)
        return;

    emit selectedPickerChanged(type);
    comboBox->setCurrentIndex(0);
}

void reset()
{
    // TODO reset combobox
}

private:
    QComboBox* comboBox;
};