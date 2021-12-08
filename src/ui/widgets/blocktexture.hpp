// -- Block Texture widget
// Displays a block texture, and has some button-like functionality

#include <QWidget>

class EditorState;
class QPixmap;

class BlockTexture : public QWidget
{
	Q_OBJECT

  public:
	BlockTexture( EditorState *editorState, uint16_t id, QWidget *parent = 0 );

	void paintEvent( QPaintEvent *event );
	void mousePressEvent( QMouseEvent *event );
	void mouseDoubleClickEvent( QMouseEvent *event );

  public slots:
	void setBlockId( uint16_t blockId );
	uint16_t getBlockId();

	int heightForWidth( int width ) const;
	bool hasHeightForWidth() const;

  signals:
	void clicked();
	void doubleClicked();

  private:
	EditorState *m_editorState;
	QPixmap m_texture;

	uint16_t m_id;
};