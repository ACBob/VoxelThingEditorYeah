// -- Block Properties
// A dialog that displays the properties of the block
// And a helpful input for the metadata, with an option to use raw.

#include <QComboBox>
#include <QDialog>

#include "editor/defs.hpp"

class ColorPicker;
class BetterSlider;
class QLineEdit;
class QDial;
class QCheckBox;
class QFormLayout;
class QGridLayout;
class QLabel;

class BlockPropertyDialog : public QDialog
{
	Q_OBJECT;

  public:
	BlockPropertyDialog( BlockDefs *defs, uint16_t id, uint16_t meta, QWidget *parent = 0 );

	uint16_t getChosenId();
	uint16_t getChosenMeta();

  private slots:
	void onIdSelectionChange( int index );

	void onMetaChangeLineEdit( const QString &text );
	void onMetaChangeSlider( int value );
	void onMetaChangeDial( int value );
	void onMetaChangeCheckBox( bool checked );
	void onMetaChangeComboBox( int index );
	void onMetaChangeColorPicker( const QColor &color );
	// TODO: bitfield

  private:
	void showAppropriateMetaThingie();

	QComboBox *m_idPicker;
	BlockDefs *defs;

	uint16_t id;
	uint16_t meta;

	QFormLayout *m_layout;

	QLabel *m_metaLabel;

	QLineEdit *m_metaRaw;
	ColorPicker *m_metaColor;
	QDial *m_metaDirectionHorz; // Used also for just direction
	QDial *m_metaDirectionVert;
	BetterSlider *m_metaLevel;
	QCheckBox *m_metaToggle;

	QWidget *m_metaBitField;
	QList<QCheckBox *> m_metaBitFieldList;
	QGridLayout *m_metaBitFieldLayout;

	QComboBox *m_metaOtherBlock;
};