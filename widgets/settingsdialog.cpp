#include "settingsdialog.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QWidget>

#include <QSettings>

#include "colorpicker.hpp"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QSettings settings;
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
    QVBoxLayout *generalLayout = new QVBoxLayout(tabGeneral);
    tabGeneral->setLayout(generalLayout);

    QLabel *label = new QLabel(tr("Grid Color:"), tabGeneral);
    generalLayout->addWidget(label);

    m_gridColorPicker = new ColorPicker(tabGeneral);
    m_gridColorPicker->enableAlpha();
    m_gridColorPicker->setColor(settings.value("gridColor", QColor(Qt::gray)).value<QColor>());
    generalLayout->addWidget(m_gridColorPicker);

    QLabel *label2 = new QLabel(tr("Void Color:"), tabGeneral);
    generalLayout->addWidget(label2);

    m_voidColorPicker = new ColorPicker(tabGeneral);
    m_voidColorPicker->setColor(settings.value("voidColor", QColor(Qt::black)).value<QColor>());
    generalLayout->addWidget(m_voidColorPicker);
}

void SettingsDialog::accept()
{
    QSettings settings;
    settings.setValue("gridColor", m_gridColorPicker->getColor());
    settings.setValue("voidColor", m_voidColorPicker->getColor());
    QDialog::accept();
}