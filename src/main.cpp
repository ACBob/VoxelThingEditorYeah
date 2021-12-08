#include <QApplication>

#include "ui/mainwindow.hpp"

#include "editor/editorstate.hpp"

int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );

	QCoreApplication::setOrganizationName( "ActuatingLemons" );
	QCoreApplication::setApplicationName( "VoxelThingEditorYeah" );

	// Set icon
	app.setWindowIcon( QIcon( ":/img/logo.png" ) );

	// Create editor state
	EditorState editorState;
	editorState.filename		 = "";
	editorState.world			 = nullptr;
	editorState.tool			 = nullptr;
	editorState.chosenBlockType	 = 1;
	editorState.chosenBlockMeta	 = 0;
	editorState.blockTexturePath = ":/img/texatlas_internal.png";

	// TODO: load the following from the QSettings
	editorState.gameDefs = definitions::LoadGameDefs(":/example/games.toml");
	editorState.game = &editorState.gameDefs->begin().value();
	editorState.blockDefs = editorState.game->blockDefs;

	auto pEditor = new MainWindow( &editorState, nullptr );
	pEditor->show();

	return app.exec();
}