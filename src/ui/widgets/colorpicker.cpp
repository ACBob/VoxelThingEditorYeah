#include "colorpicker.hpp"

#include <QColorDialog>

#include <QMouseEvent>
#include <QPainter>

#include <QDebug>

ColorPicker::ColorPicker( QWidget *parent ) : QWidget( parent )
{
	setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	setMinimumHeight( 20 );
	setMaximumHeight( 20 );

	m_color		 = QColor( 0, 0, 0 );
	m_allowAlpha = false;
}

ColorPicker::~ColorPicker() {}

void ColorPicker::setColor( const QColor &color )
{
	// Emit signal
	m_color = color;
	update();

	emit colorChanged( color );
}

void ColorPicker::setColorFromVTY( uint16_t vty )
{
	// VTY uses RGBA, Qt uses AARRGGBB.

	uint8_t r = ( vty >> 12 ) & 0xF;
	uint8_t g = ( vty >> 8 ) & 0xF;
	uint8_t b = ( vty >> 4 ) & 0xF;
	uint8_t a = m_allowAlpha ? ( vty ) & 0xF : 0xF;

	// qDebug() << "Color:" << r << g << b << a;

	// Zoop it to 4 bits
	r *= 17;
	g *= 17;
	b *= 17;
	a *= 17;

	// qDebug() << "Color:" << r << g << b << a;

	// now pack
	uint32_t qtColor = (a << 24) | ( r << 16 ) | ( g << 8 ) | b;

	// qDebug() << "Color:" << qtColor << QString("%1").arg(qtColor, 0, 16);

	m_color = QColor::fromRgba(qtColor);
}

QColor ColorPicker::getColor() { return m_color; }

uint16_t ColorPicker::getVTYColor()
{
	// QColor uses QRGB.
	// According to https://doc.qt.io/qt-5/qcolor.html#QRgb-typedef, it is AARRGGBB.
	// VoxelThingYeah uses RGBA - both 16bit and a different order.
	// We lose some precision, but also we'd have too many bits if meta was 32bit.
	QRgb color = m_color.rgba();

	uint8_t a = m_allowAlpha ? (color & 0xFF000000) >> 24 : 0xFF;
	uint8_t r = (color & 0x00FF0000) >> 16;
	uint8_t g = (color & 0x0000FF00) >> 8;
	uint8_t b = (color & 0x000000FF);

	// qDebug() << "Color:" << a << r << g << b;

	// Smash it down to half the bits
	// I could technically do this in the first step but this is more readable
	// We use the most significant bits
	a = (a & 0xF0) >> 4;
	r = (r & 0xF0) >> 4;
	g = (g & 0xF0) >> 4;
	b = (b & 0xF0) >> 4;

	// qDebug() << "Color:" << a << r << g << b;

	// merge it into one, final colour
	uint16_t vtyColor = (r << 12) | (g << 8) | (b << 4) | a;

	// qDebug() << "Color:" << vtyColor << QString("%1").arg(vtyColor, 0, 16);

	return vtyColor;
}

void ColorPicker::enableAlpha() { m_allowAlpha = true; }

void ColorPicker::disableAlpha() { m_allowAlpha = false; }

void ColorPicker::paintEvent( QPaintEvent *event )
{
	QPainter painter( this );

	// checkerboard
	if ( m_allowAlpha )
	{
		for ( int i = 0; i < width(); i += 10 )
		{
			for ( int j = 0; j < height(); j += 10 )
			{
				if ( ( i / 10 + j / 10 ) % 2 == 0 )
				{
					painter.setPen( Qt::white );
					painter.setBrush( Qt::white );
				}
				else
				{
					painter.setPen( Qt::gray );
					painter.setBrush( Qt::gray );
				}
				painter.drawRect( i, j, 10, 10 );
			}
		}
	}

	// fill with color
	painter.setPen( Qt::black );
	painter.setBrush( m_color );
	painter.drawRect( 0, 0, width(), height() );
}

void ColorPicker::mousePressEvent( QMouseEvent *event )
{
	QColor color = QColorDialog::getColor( m_color, this, tr( "Select Color" ),
										   m_allowAlpha ? QColorDialog::ShowAlphaChannel : QFlag( 0 ) );
	if ( color.isValid() )
		setColor( color );
}