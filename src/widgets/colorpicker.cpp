#include "colorpicker.hpp"

#include <QColorDialog>

#include <QPainter>
#include <QMouseEvent>


ColorPicker::ColorPicker(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(20);
    setMaximumHeight(20);

    m_color = QColor(0, 0, 0);
    m_allowAlpha = false;
}

ColorPicker::~ColorPicker()
{
}

void ColorPicker::setColor(const QColor &color)
{
    m_color = color;
    update();
}

QColor ColorPicker::getColor()
{
    return m_color;
}

void ColorPicker::enableAlpha()
{
    m_allowAlpha = true;
}

void ColorPicker::disableAlpha()
{
    m_allowAlpha = false;
}

void ColorPicker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // checkerboard
    if (m_allowAlpha)
    {
        for (int i = 0; i < width(); i += 10)
        {
            for (int j = 0; j < height(); j += 10)
            {
                if ((i / 10 + j / 10) % 2 == 0)
                {
                    painter.setPen(Qt::white);
                    painter.setBrush(Qt::white);
                }
                else
                {
                    painter.setPen(Qt::gray);
                    painter.setBrush(Qt::gray);
                }
                painter.drawRect(i, j, 10, 10);
            }
        }
    }

    // fill with color
    painter.setPen(Qt::black);
    painter.setBrush(m_color);
    painter.drawRect(0, 0, width(), height());
}

void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    QColor color = QColorDialog::getColor(m_color, this, tr("Select Color"), m_allowAlpha ? QColorDialog::ShowAlphaChannel : QFlag(0));
    if (color.isValid())
        setColor(color);
}