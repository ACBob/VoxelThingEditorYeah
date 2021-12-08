// -- Settings Dialog
// holds all settings and provides a dialog to change them

#include <QDialog>

class ColorPicker;
class EditorState;
class QComboBox;

class SettingsDialog : public QDialog
{
	Q_OBJECT

  public:
	SettingsDialog( EditorState *editorState, QWidget *parent = 0 );

  public slots:
	void accept();

  private:
	ColorPicker *m_gridColorPicker;
	ColorPicker *m_voidColorPicker;
	QComboBox *m_gameDefComboBox;

	EditorState *m_editorState;
};