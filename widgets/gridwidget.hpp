// -- Grid widget
// Doesn't use OpenGL, instead uses the QT paint engine

#include <QWidget>

class GridWidget : public QWidget
{
    Q_OBJECT

    public:
        GridWidget(QWidget *parent = 0);

        void setGridColor(QColor color);
        void setGridSpacing(int spacing);

    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);

        // scroll
        void wheelEvent(QWheelEvent *event);

    private:
        QColor m_gridColor;
        int m_gridSpacing;

        QPoint m_lastMousePos;

        // Position to start drawing the grid
        int m_gridX;
        int m_gridY;
        // Grid Position that is selected
        int m_selGridX;
        int m_selGridY;
};