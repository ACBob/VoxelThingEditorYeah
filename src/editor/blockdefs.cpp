#include "blockdefs.hpp"

#include "toml.hpp"

#include <QMap>

#include <QFile>

#include <QDebug>

#include <QDialog>

BlockDefs LoadBlockDefs( const QString &path )
{
	// Open for reading
	QFile file( path );
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		// qWarning() << "Failed to open blockdefs file" << path;
		// Display a warning popup
		QDialog *dialog = new QDialog();
		dialog->setWindowTitle( "Failed to open blockdefs file" );
		dialog->setWindowModality( Qt::WindowModal );
		dialog->setAttribute( Qt::WA_DeleteOnClose );
		dialog->show();
		return {};
	}

	// Read the file
	QString contents = file.readAll();
	file.close();

	// Parse the TOML
	toml::table root = toml::parse( contents.toStdString() );

	BlockDefs defs;

	for ( auto &k : root )
	{
		BlockDef def;

		// the ID is the key
		int id = QString( k.first.c_str() ).toInt();

		qDebug() << "Loading blockdef for" << id;

		// Get the blockdef
		toml::table *blockdef = k.second.as_table();

		// Get the name
		def.name = QString::fromStdString( blockdef->get( "name" )->value_or<std::string>( "UnNamed" ) );

		// Get the texture position
		QPoint p;
		toml::array *pos = blockdef->get( "texturePos" )->as_array();
		p.setX( pos->get( 0 )->value_or<int>( 0 ) );
		p.setY( pos->get( 1 )->value_or<int>( 0 ) );

		def.texturePos = p;

		// Get the meta type
		BlockMetaType metaType = META_NONE;
		QString meta;
		if ( blockdef->get("metaUse") )
			meta = QString::fromStdString( blockdef->get( "metaUse" )->value_or<std::string>( "None" ) );
		else
			meta = "None";

		// TODO: any better way than else ifs?
		if ( meta == "None" )
		{
			metaType = META_NONE;
		}
		else if ( meta == "Color" )
		{
			metaType = META_COLOR;
		}
		else if ( meta == "DirectionHoriz" )
		{
			metaType = META_DIRECTION_HORIZ;
		}
		else if ( meta == "DirectionVert" )
		{
			metaType = META_DIRECTION_VERT;
		}
		else if ( meta == "Direction" )
		{
			metaType = META_DIRECTION;
		}
		else if ( meta == "Level" )
		{
			metaType = META_LEVEL;
		}
		else if ( meta == "Toggle" )
		{
			metaType = META_TOGGLE;
		}
		else if ( meta == "BitField" )
		{
			metaType = META_BITFIELD;
		}
		else if ( meta == "OtherBlock" )
		{
			metaType = META_OTHERBLOCK;
		}
		else
		{
			qWarning() << "Unknown meta type" << meta;
		}

		def.metaType = metaType;

		// min/max
		if ( blockdef->get("metaMin") )
			def.metaMin = blockdef->get( "metaMin" )->value_or<int>( 0 );
		else
			def.metaMin = 0;
		if ( blockdef->get("metaMax") )
			def.metaMax = blockdef->get( "metaMax" )->value_or<int>( UINT16_MAX );
		else
			def.metaMax = UINT16_MAX;

		defs[id] = def;
	}

	return defs;
}