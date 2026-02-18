#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QRadioButton>

class PointPickerConfigurationTool: public QWidget
{
    Q_OBJECT
public:
    PointPickerConfigurationTool(QWidget* parent): QWidget(parent)
    {
        QIntValidator* rgbValidator = new QIntValidator(0, 255, this);
        QFormLayout* formLayout = new QFormLayout;

        QLineEdit* nameEdit = new QLineEdit;

        QLineEdit* rEdit = new QLineEdit();
        rEdit->setValidator(rgbValidator);
        QLineEdit* gEdit = new QLineEdit();
        gEdit->setValidator(rgbValidator);
        QLineEdit* bEdit = new QLineEdit();
        bEdit->setValidator(rgbValidator);

        QHBoxLayout *colorLayout = new QHBoxLayout;
        colorLayout->addWidget(rEdit);
        colorLayout->addWidget(gEdit);
        colorLayout->addWidget(bEdit);
        colorLayout->setSpacing(5);

        QWidget *colorWidget = new QWidget;
        colorWidget->setLayout(colorLayout);

        formLayout->addRow("Name: ", nameEdit);
        formLayout->addRow("Color: ", colorWidget);

        QRadioButton* radioButton = new QRadioButton("Enabled");

        QVBoxLayout* radioLayout = new QVBoxLayout;
        radioLayout->addStretch();
        radioLayout->addWidget(radioButton);
        radioLayout->addStretch();

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->addLayout(formLayout);
        mainLayout->addLayout(radioLayout);

        setLayout(mainLayout);
    }
};