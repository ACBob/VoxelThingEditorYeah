#include <QApplication>

#include "ui/mainwindow.hpp"

#include "world/loading/worldformat.hpp";

int main( int argc, char *argv[] )
{
	// quick test for world loading
	CWorld *world = new CWorld();
	Meegreef1Format *fmt = new Meegreef1Format();
	fmt->Load( world, "world.dat" );

	QApplication app( argc, argv );

	QCoreApplication::setOrganizationName( "ActuatingLemons" );
	QCoreApplication::setApplicationName( "VoxelThingEditorYeah" );

	auto pEditor = new MainWindow( nullptr );
	pEditor->show();

	return app.exec();
}