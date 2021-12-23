#include "defs.hpp"

#include "toml.hpp"

#include <QMap>
#include <QFile>
#include <QDebug>
#include <QDialog>
#include <QDir>


BlockDefs *definitions::LoadBlockDefs( const QString &path )
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

	BlockDefs *defs = new BlockDefs();

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
		if ( blockdef->get( "metaUse" ) )
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
		if ( blockdef->get( "metaMin" ) )
			def.metaMin = blockdef->get( "metaMin" )->value_or<int>( 0 );
		else
			def.metaMin = 0;
		if ( blockdef->get( "metaMax" ) )
			def.metaMax = blockdef->get( "metaMax" )->value_or<int>( UINT16_MAX );
		else
			def.metaMax = UINT16_MAX;

		// isLiquid
		if ( blockdef->get( "isLiquid" ) )
			def.isLiquid = blockdef->get( "isLiquid" )->value_or<bool>( false );
		else
			def.isLiquid = false;

		// liquidSource
		if ( blockdef->get( "liquidSource" ) )
			def.liquidSource = blockdef->get( "liquidSource" )->value_or<uint16_t>( 0 );
		else
			def.liquidSource = 0;

		// liquidFlow
		if ( blockdef->get( "liquidFlow" ) )
			def.liquidFlow = blockdef->get( "liquidFlow" )->value_or<uint16_t>( 0 );
		else
			def.liquidFlow = 0;

		defs->insert( id, def );
	}

	return defs;
}

EntityDefs *definitions::LoadEntityDefs(const QString &path)
{
	// Open for reading
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		// qWarning() << "Failed to open entitydefs file" << path;
		// Display a warning popup
		QDialog *dialog = new QDialog();
		dialog->setWindowTitle("Failed to open entitydefs file");
		dialog->setWindowModality(Qt::WindowModal);
		dialog->setAttribute(Qt::WA_DeleteOnClose);
		dialog->show();
		return {};
	}

	// Read the file
	QString contents = file.readAll();
	file.close();

	// Parse the TOML
	toml::table root = toml::parse(contents.toStdString());

	EntityDefs *defs = new EntityDefs();

	for (auto &k : root)
	{
		EntityDef def;

		// the ID is the key
		QString id = QString(k.first.c_str());

		qDebug() << "Loading entitydef for" << id;

		// Get the entitydef
		toml::table *entitydef = k.second.as_table();

		if (entitydef->get("friendlyName"))
		{
			QString friendlyName = QString::fromStdString(entitydef->get("friendlyName")->value_or<std::string>(""));
			if (friendlyName.isEmpty())
			{
				qWarning() << "Entitydef" << id << "has no friendly name";
				friendlyName = id;
			}
			def.friendlyName = friendlyName;
		}
		else
		{
			qWarning() << "Entitydef" << id << "has no friendly name";
			def.friendlyName = id;
		}

		// store visualtype
		if (entitydef->get("visualType"))
		{
			QString visualType = QString::fromStdString(entitydef->get("visualType")->value_or<std::string>(""));
			if (visualType.isEmpty())
			{
				qWarning() << "Entitydef" << id << "has no visual type";
				visualType = "sprite";
			}
			if (visualType == "model")
			{
				qWarning() << "Model entitydefs not yet supported";
				visualType = "sprite";
			}
			def.visualType = visualType;
		}
		else
		{
			qWarning() << "Entitydef" << id << "has no visual type";
			def.visualType = "sprite";
		}

		// store texture
		if (entitydef->get("texture"))
		{
			QString texture = QString::fromStdString(entitydef->get("texture")->value_or<std::string>(""));
			if (texture.isEmpty())
			{
				qWarning() << "Entitydef" << id << "has no texture";
				texture = "default";
			}
			def.texture = texture;
		}
		else
		{
			qWarning() << "Entitydef" << id << "has no texture";
			def.texture = "default";
		}

		// if we have an absorb value
		if (entitydef->get("absorb"))
		{
			// Merge with specified entitydef
			QString absorb = QString::fromStdString(entitydef->get("absorb")->value_or<std::string>(""));
			if (!defs->contains(absorb))
			{
				qWarning() << "Entitydef" << id << "has invalid absorb value" << absorb;
				continue;
			}

			EntityDef other = defs->value(absorb);

			// Merge our properties with the other
			def.properties.unite(other.properties);
		}

		// Now work on properties
		toml::table *propertyTable = entitydef->get("properties")->as_table();
		for (auto &k : *propertyTable)
		{
			// Key is the name
			QString name = QString(k.first.c_str());

			// The value is a list of strings
			toml::array *values = k.second.as_array();

			qDebug() << "Property" << name << "has" << values->size() << "values";

			if (values->size() < 4 || values->size() > 5)
			{
				qWarning() << "Property" << name << "has invalid number of values";
				continue;
			}

			// Create a new property
			EntityProperty prop;

			prop.name = name;
			prop.type = QString::fromStdString(values->get(0)->value_or<std::string>(""));
			if (prop.type != "LIST" && values->size() < 5)
			{
				qWarning() << "Property" << name << "has invalid number of values";
				continue;
			}
			prop.description = QString::fromStdString(values->get(1)->value_or<std::string>(""));
			prop.min = QString::fromStdString(values->get(2)->value_or<std::string>("")); // in a list, is actually the list of values
			prop.max = QString::fromStdString(values->get(3)->value_or<std::string>("")); // in a list, is actually the default value

			if (values->size() == 5)
			{
				prop.defaultValue = QString::fromStdString(values->get(4)->value_or<std::string>(""));
			}

			// Add the property
			def.properties.insert(name, prop);
		}

		defs->insert(id, def);
	}

	return defs;
}

void definitions::LoadGameDefs( const QString &path, GameDefs &defs )
{
	// Open for reading
	QFile file( path );
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		qWarning() << "Failed to open game defs file" << path;
		return;
	}

	// Read the file
	QString contents = file.readAll();
	file.close();

	// Parse the TOML
	toml::table root = toml::parse( contents.toStdString() );

	for ( auto &k : root )
	{
		GameDef def;

		// the ID is the key
		QString id = QString( k.first.c_str() );

		qDebug() << "Loading game def for" << id;

		// Get the game def
		toml::table *gamedef = k.second.as_table();

		// Get the name
		def.id = id;
		def.name = QString::fromStdString( gamedef->get( "name" )->value_or<std::string>( "UnNamed" ) );

		def.absolutePath = QString::fromStdString( gamedef->get( "path" )->value_or<std::string>( "" ) );
		if ( def.absolutePath.isEmpty() )
		{
			qWarning() << "Game def" << id << "has no absolute path";
			continue;
		}

		def.workingPath = QString::fromStdString( gamedef->get( "working_dir" )->value_or<std::string>( "" ) );
		if ( def.workingPath.isEmpty() )
		{
			qWarning() << "Game def" << id << "has no working dir";
		}

		def.texturePath = QString::fromStdString( gamedef->get( "texture_path" )->value_or<std::string>( "" ) );
		if ( def.texturePath.isEmpty() )
		{
			qWarning() << "Game def" << id << "has no texture path";
		}

		def.filePath = QString::fromStdString( gamedef->get( "file" )->value_or<std::string>( "" ) );
		if ( def.filePath.isEmpty() )
		{
			qWarning() << "Game def" << id << "has no file path";
		}
		else
		{
			// attempt to load the file
			QString path = QDir( def.absolutePath ).absoluteFilePath( def.filePath );
			LoadGameDef( def.filePath, def );
		}

		QString blockdefs = QString::fromStdString( gamedef->get( "blockdefs" )->value_or<std::string>( "" ) );
		if ( blockdefs.isEmpty() )
		{
			qWarning() << "Game def" << id << "has no blockdefs";
			def.blockDefs = nullptr;
		}
		else
		{
			// attempt to load the file
			QString path = QDir( def.absolutePath ).absoluteFilePath( blockdefs );
			def.blockDefs = LoadBlockDefs( path );
		}

		QString entitydefs = QString::fromStdString( gamedef->get( "entdefs" )->value_or<std::string>( "" ) );
		if ( entitydefs.isEmpty() )
		{
			qWarning() << "Game def" << id << "has no entitydefs";
			def.entityDefs = nullptr;
		}
		else
		{
			// attempt to load the file
			QString path = QDir( def.absolutePath ).absoluteFilePath( entitydefs );
			def.entityDefs = LoadEntityDefs( path );
		}

		defs.insert( id, def );
	}
}

void definitions::LoadGameDef( const QString &path, GameDef &def )
{
	// Open for reading
	QFile file( path );
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		qWarning() << "Failed to open game def file" << path;
		return;
	}

	// Read the file
	QString contents = file.readAll();

	// Parse the TOML
	toml::table root = toml::parse( contents.toStdString() );

	// Same keys as the game def (just-about)
	if ( root.get( "name" ) )
		def.name = QString::fromStdString( root.get( "name" )->value_or<std::string>( "UnNamed" ) );
	if ( root.get( "workingDir" ) )
		def.workingPath = QString::fromStdString( root.get( "working_dir" )->value_or<std::string>( "" ) );
	if ( root.get( "texturePath" ) )
		def.texturePath = QString::fromStdString( root.get( "texture_path" )->value_or<std::string>( "" ) );
	if ( root.get("blockdefs") )
		def.blockDefs = LoadBlockDefs( QDir( def.absolutePath ).absoluteFilePath( QString::fromStdString( root.get( "blockdefs" )->value_or<std::string>( "" ) ) ) );
	if ( root.get("entdefs") )
		def.entityDefs = LoadEntityDefs( QDir( def.absolutePath ).absoluteFilePath( QString::fromStdString( root.get( "entdefs" )->value_or<std::string>( "" ) ) ) );
};