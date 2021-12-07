// -- Color picker
// Shows a little box with a color preview, and when clicked it shows a color picker.

#include <QWidget>

class ColorPicker : public QWidget
{
	Q_OBJECT

  public:
	ColorPicker( QWidget *parent = 0 );
	~ColorPicker();

	void enableAlpha();
	void disableAlpha();

	public slots:
	void setColor( const QColor &color );
	void setColorFromVTY( uint16_t vty_color ); // converts VoxelThingYeah 16bit color to Qt color
	QColor getColor();
	uint16_t getVTYColor(); // converts Qt color to VoxelThingYeah 16bit color

	signals:
	void colorChanged( const QColor &color );


  protected:
	void paintEvent( QPaintEvent *event );
	void mousePressEvent( QMouseEvent *event );

  private:
	QColor m_color;
	bool m_allowAlpha;
};