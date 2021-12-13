#include "blocktexture.hpp"

#include <QPainter>
#include <QPixmap>

#include "editor/editorstate.hpp"

#include "render/texture.hpp"

BlockTexture::BlockTexture( EditorState *editorState, uint16_t id, QWidget *parent )
	: QWidget( parent ), m_editorState( editorState )
{
	m_texture.load( editorState->m_sBlockTexturePath );

	m_id = id;
}

int BlockTexture::heightForWidth( int width ) const { return width; }

bool BlockTexture::hasHeightForWidth() const { return true; }

void BlockTexture::paintEvent( QPaintEvent *event )
{
	// get UV coordinates
	Vector4f uv = render::getUV( m_editorState->m_pBlockDefs, m_id );
	uv.x *= m_texture.width();
	uv.y *= m_texture.height();
	uv.z = m_texture.height() / 16.0f;
	uv.w = m_texture.width() / 16.0f;

	QPainter painter( this );

	// draw texture
	painter.drawPixmap( 0, 0, width(), height(), m_texture, uv.x, uv.y, uv.z, uv.w );

	// draw border
	painter.setPen( Qt::black );
	painter.drawRect( 0, 0, width() - 1, height() - 1 );
}

void BlockTexture::mousePressEvent( QMouseEvent *event ) { emit clicked(); }

void BlockTexture::mouseDoubleClickEvent( QMouseEvent *event ) { emit doubleClicked(); }

void BlockTexture::setBlockId( uint16_t blockId )
{
	m_id = blockId;
	update();
}

uint16_t BlockTexture::getBlockId() { return m_id; }