#include "gridwidget.hpp"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAutoFillBackground(true);

    m_gridSpacing = 10;
    m_gridColor = QColor(Qt::lightGray);
    m_gridX = 0;
    m_gridY = 0;
    m_selGridX = 0;
    m_selGridY = 0;
}

void GridWidget::setGridColor(QColor color)
{
    m_gridColor = color;
}

void GridWidget::setGridSpacing(int spacing)
{
    m_gridSpacing = spacing;
}

void GridWidget::paintEvent( QPaintEvent *event )
{
    QPainter painter(this);

    // Draw the bounds of the widget
    painter.setPen(Qt::black);
    painter.drawRect(0, 0, width()-1, height()-1);
    // Fill the background
    painter.fillRect(0, 0, width(), height(), QBrush(Qt::black));

    // Draw the grid
    painter.setPen(m_gridColor);
    // also show m_gridX and m_gridY
    // which are the coords of the top left corner of the grid
    for (int x = m_gridX; x < width(); x += m_gridSpacing)
    {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = m_gridY; y < height(); y += m_gridSpacing)
    {
        painter.drawLine(0, y, width(), y);
    }

    // Draw the focus rectangle
    if (hasFocus())
    {
        painter.setPen(Qt::blue);
        painter.drawRect(0, 0, width()-1, height()-1);
    }

    // // Draw the selected grid
    // painter.fillRect((m_selGridX * m_gridSpacing) + m_gridX, (m_selGridY * m_gridSpacing) + m_gridY, m_gridSpacing, m_gridSpacing, QBrush(Qt::blue));
}

void GridWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Middle mouse button is used to drag the grid
    if (event->buttons() & Qt::MidButton)
    {
        int dx = event->x() - m_lastMousePos.x();
        int dy = event->y() - m_lastMousePos.y();

        m_gridX += dx;
        m_gridY += dy;

        update();

        // move the cursor back
        m_lastMousePos = event->pos();
        QCursor::setPos(mapToGlobal(m_lastMousePos));

        // qDebug() << "GridWidget::mouseMoveEvent() - m_gridX:" << m_gridX << "m_gridY:" << m_gridY;
    }
    m_lastMousePos = event->pos();
}

void GridWidget::mousePressEvent(QMouseEvent *event)
{
    // Figure out the grid coords of the click
    if (event->button() == Qt::LeftButton)
    {
        m_selGridX = event->x() % m_gridSpacing;
        m_selGridY = event->y() % m_gridSpacing;
        update();
    }
}

void GridWidget::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        if (event->delta() > 0)
        {
            m_gridSpacing += 1;
        }
        else
        {
            m_gridSpacing -= 1;
        }
        m_gridSpacing = qMax(1, m_gridSpacing);

        update();
    }
}