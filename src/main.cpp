#include <QApplication>

#include "ui/mainwindow.hpp"

#include "editor/editorstate.hpp"

int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );

	QCoreApplication::setOrganizationName( "ActuatingLemons" );
	QCoreApplication::setApplicationName( "VoxelThingEditorYeah" );

	// Create editor state
	EditorState editorState;
	editorState.blockDefs = nullptr;
	editorState.filename = "";
	editorState.world = nullptr;
	editorState.tool = nullptr;
	editorState.chosenBlockType = 1;
	editorState.chosenBlockMeta = 0;
	editorState.blockTexturePath = ":/img/texatlas_internal.png";

	auto pEditor = new MainWindow( &editorState, nullptr );
	pEditor->show();

	return app.exec();
}