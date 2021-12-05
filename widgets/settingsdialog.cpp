#include "settingsdialog.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QWidget>

#include "colorpicker.hpp"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Settings"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QTabWidget *tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    QHBoxLayout *buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(buttonLayout);

    QPushButton *okButton = new QPushButton(tr("OK"), this);
    buttonLayout->addWidget(okButton);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    buttonLayout->addWidget(cancelButton);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    // Tabs
    QWidget *tabGeneral = new QWidget(this);
    tabWidget->addTab(tabGeneral, tr("General"));

    // General
    QVBoxLayout *layoutGeneral = new QVBoxLayout(tabGeneral);
    tabGeneral->setLayout(layoutGeneral);
    QLabel *labelGeneral = new QLabel(tr("General"), tabGeneral);
    layoutGeneral->addWidget(labelGeneral);
    QPushButton *buttonGeneral = new QPushButton(tr("OK"), tabGeneral);
    layoutGeneral->addWidget(buttonGeneral);

    ColorPicker *colorPicker = new ColorPicker(this);
    layoutGeneral->addWidget(colorPicker);

    ColorPicker *colorPicker2 = new ColorPicker(this);
    colorPicker2->enableAlpha();
    layoutGeneral->addWidget(colorPicker2);
}