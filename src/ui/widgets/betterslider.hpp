// -- Better slider
// A QSlider but it has a label showing the current value.
// It is shrunken to the Right a bit to make room for the label.

#include <QSlider>

class QLabel;

// functionality 100% the same as QSlider, we just add a label
class BetterSlider : public QSlider
{
	Q_OBJECT

  public:
	BetterSlider( QWidget *parent = 0 );
	~BetterSlider();

	void paintEvent( QPaintEvent *event );

  private:
	QLabel *m_label;
};