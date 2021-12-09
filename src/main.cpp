#include <QApplication>

#include <QUndoStack>

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
	editorState.blockDefs = nullptr;
	editorState.filename = "";
	editorState.world = nullptr;
	editorState.tool = nullptr;
	editorState.chosenBlockType = 1;
	editorState.chosenBlockMeta = 0;
	editorState.blockTexturePath = ":/img/texatlas_internal.png";
	editorState.undoStack = new QUndoStack( &editorState );
	editorState.undoStack->setUndoLimit( 100 );
	editorState.undoStack->clear();

	auto pEditor = new MainWindow( &editorState, nullptr );
	pEditor->show();

	return app.exec();
}