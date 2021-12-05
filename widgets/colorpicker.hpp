// -- Color picker
// Shows a little box with a color preview, and when clicked it shows a color picker.

#include <QWidget>

class ColorPicker : public QWidget
{
    Q_OBJECT

    public:
        ColorPicker(QWidget *parent = 0);
        ~ColorPicker();

        void setColor(const QColor &color);
        QColor getColor();

        void enableAlpha();
        void disableAlpha();

    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);

    private:
        QColor m_color;
        bool m_allowAlpha;
};