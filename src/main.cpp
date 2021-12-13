#include <QApplication>

#include <QUndoStack>

#include "ui/mainwindow.hpp"

#include "editor/editorstate.hpp"

#include <QSplashScreen>
#include <QTimer>
#include <QDesktopWidget>

int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );

	QCoreApplication::setOrganizationName( "ActuatingLemons" );
	QCoreApplication::setApplicationName( "VoxelThingEditorYeah" );

	// Set icon
	app.setWindowIcon( QIcon( ":/img/logo.png" ) );

	// Display a splash screen for 2 seconds
	QSplashScreen splash( QPixmap( ":/img/splash.png" ) );
	splash.show();
	app.processEvents();
	QTimer::singleShot( 2000, &splash, SLOT( close() ) );

	// Create editor state
	EditorState editorState;

	// TODO: load the following from the QSettings
	// TODO: Handle in constructor
	editorState.m_pGameDefs = definitions::LoadGameDefs(":/example/games.toml");
	editorState.m_pGame = &editorState.m_pGameDefs->begin().value();
	editorState.m_pBlockDefs = editorState.m_pGame->blockDefs;

	auto pEditor = new MainWindow( &editorState, nullptr );

	// Just looks nice
	pEditor->resize( app.desktop()->screen()->rect().width() * 0.7, app.desktop()->screen()->rect().height() * 0.7 );
	pEditor->move( app.desktop()->screen()->rect().center() - pEditor->rect().center() );
	pEditor->show();

	return app.exec();
}