// -- Settings Dialog
// holds all settings and provides a dialog to change them

#include <QDialog>

class ColorPicker;

class SettingsDialog : public QDialog
{
    Q_OBJECT

    public:
        SettingsDialog(QWidget *parent = 0);

    public slots:
        void accept();
    
    private:
        ColorPicker *m_gridColorPicker;
        ColorPicker *m_voidColorPicker;
};