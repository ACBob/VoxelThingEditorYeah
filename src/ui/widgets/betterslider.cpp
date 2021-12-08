#include "betterslider.hpp"

#include <QPainter>

BetterSlider::BetterSlider( QWidget *parent ) : QSlider( parent ) {}

BetterSlider::~BetterSlider() {}

// TODO: may be hard to see, do better
void BetterSlider::paintEvent( QPaintEvent *event )
{
	QSlider::paintEvent( event );

	QPainter painter( this );
	// set pen colour to text colour
	QColor textColour = palette().color( QPalette::Text );
	painter.setPen( textColour );
	painter.drawText( rect(), Qt::AlignCenter, QString::number( value() ) );
}